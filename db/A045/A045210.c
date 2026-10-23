/*
 * A045210  Numbers whose base-5 representation contains exactly two 0's and
 *          no 4's.
 * https://oeis.org/A045210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   count_remaining
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth integer whose base‑5 representation contains
 * exactly two 0's and no 4's.
 * Computed by combinatorial ranking of base‑5 digits. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_remaining.h"

/* precomputed digit choices for the most‑significant position and the
   remaining positions */
static const int msd_digits[3] = {1, 2, 3};
static const int rest_digits[4] = {0, 1, 2, 3};

/* Return the n‑th term of the sequence (a(1) = 25, a(2) = 50, …). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* out of range; value arbitrary */

    /* ---------- find length L of the base‑5 representation ---------- */
    idx_t L = 3;          /* minimal length with two zeros */
    idx_t cumulative = 0;
    while (true) {
        idx_t cnt_len = 3 * count_remaining(L - 1, 2); /* 3 MSB options */
        if (cumulative + cnt_len >= n)
            break;
        cumulative += cnt_len;
        ++L;
    }
    idx_t offset = n - cumulative;           /* 1‑based rank within length L */

    /* ---------- construct the number digit by digit ---------- */
    idx_t zeros_used = 0;
    seq_t result = 0;

    for (idx_t pos = 0; pos < L; ++pos) {
        const int *cand = (pos == 0) ? msd_digits : rest_digits;
        int cand_cnt = (pos == 0) ? 3 : 4;

        for (int i = 0; i < cand_cnt; ++i) {
            int d = cand[i];
            /* Leading zero is forbidden */
            if (pos == 0 && d == 0)
                continue;

            idx_t zeros_left = 2 - zeros_used - (d == 0 ? 1 : 0);
            if (zeros_left > (L - pos - 1) || zeros_left < 0)
                continue;                     /* impossible choice */

            idx_t completions = count_remaining(L - pos - 1, zeros_left);

            if (offset > completions) {
                offset -= completions;
                continue;                     /* skip these completions */
            }

            /* choose this digit */
            result = result * 5 + (seq_t)d;
            if (d == 0)
                ++zeros_used;
            break;
        }
    }

    return result;
}
