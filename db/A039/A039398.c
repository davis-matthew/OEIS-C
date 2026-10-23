/*
 * A039398  Numbers whose base-10 representation has the same nonzero number
 *          of 0's and 6's.
 * https://oeis.org/A039398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039398 – Numbers whose decimal representation has the same
   non‑zero number of 0's and 6's. The function finds the n‑th such number
   by brute‑force search. */

#include <stdbool.h>
#include "../utils/digit_counts.h"

/* Return true iff x has a positive number of 0's and 6's, and those
   two counts are equal.  Uses the digit_counts helper which works for
   both native integer and arbitrary‑precision seq_t types. */
static bool has_equal_nonzero_zeros_and_six(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    unsigned zeros = (unsigned)cnt[0];
    unsigned sixes = (unsigned)cnt[6];
    return zeros > 0 && zeros == sixes;
}

/* Return the n-th term of sequence A039398 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1 – return 0 safely */

    seq_t cur = 1;                   /* candidate number */
    idx_t found = 0;                 /* how many valid numbers seen */

    while (1) {
        if (has_equal_nonzero_zeros_and_six(cur)) {
            ++found;
            if (found == n) return cur;
        }
        ++cur;
    }
}
