/*
 * A043349  Numbers having one 0 in base 5.
 * https://oeis.org/A043349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑5 representation
 *            contains exactly one 0 digit
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   pow4_fn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow4_fn.h"

/* number of ways to fill the remaining ‘pos’ positions when zero_used indicates
   whether the zero digit has already been placed. */
static inline seq_t count_completions(idx_t pos, bool zero_used) {
    if (zero_used) {                    /* zero already placed – no more zeros allowed */
        return pow4_fn(pos);             /* all remaining digits are 1..4 */
    }
    if (pos == 0) {                     /* no positions left but zero not yet placed – impossible */
        return 0;
    }
    return (seq_t)pos * pow4_fn(pos - 1);/* choose position for the zero, the rest are 1..4 */
}

/* the required function */
seq_t a(idx_t n) {
    if (n == 0) {          /* offset is 1, but safeguard */
        return 0;
    }

    /* find the length of the base‑5 representation containing the required number of terms */
    idx_t length = 2;            /* minimal length (digits 10 in base 5) */
    unsigned __int128 cumulative = 0;
    unsigned __int128 count_current = 0;

    while (1) {
        idx_t pos = length - 1;   /* positions after the first digit */
        /* count = (length-1) * 4^(length-1) */
        count_current = (unsigned __int128)pos *
                        (unsigned __int128)pow4_fn(pos);
        cumulative += count_current;
        if (cumulative >= n) {
            break;
        }
        ++length;
    }

    /* offset within numbers of this length (0‑based) */
    idx_t prev_cumulative = (idx_t)(cumulative - count_current);
    idx_t offset = (idx_t)(n - prev_cumulative - 1);

    /* build the number digit by digit */
    seq_t result = 0;
    bool zero_used = false;

    for (idx_t pos_index = 0; pos_index < length; ++pos_index) {
        int start_digit = 0;
        if (pos_index == 0) {       /* most significant digit cannot be zero */
            start_digit = 1;
        }
        for (int d = start_digit; d <= 4; ++d) {
            if (d == 0 && zero_used) {
                continue;          /* zero already used, cannot place another */
            }

            bool new_zero_used = zero_used;
            if (d == 0) {
                new_zero_used = true;
            }

            idx_t remaining = length - 1 - pos_index;
            seq_t completions = count_completions(remaining, new_zero_used);

            /* compare offset with completions, but only cast when completions ≤ offset */
            if (completions <= (seq_t)offset) {
                offset -= (idx_t)completions;
                continue;
            }

            /* choose this digit */
            result = result * 5 + (seq_t)d;
            zero_used = new_zero_used;
            break;
        }
    }

    return result;
}
