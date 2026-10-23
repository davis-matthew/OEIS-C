/*
 * A045220  Numbers whose base-5 representation contains no 1's and exactly
 *          three 2's.
 * https://oeis.org/A045220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑5 representation contains no 1’s and exactly
 *            three 2’s (OEIS A045220)
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: compute binomial coefficient C(n,k) in seq_t. */
static seq_t
binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        r = r * (n - i + 1) / i;
    }
    return r;
}

/* Helper: compute 3^e in seq_t. */
static seq_t
pow3(idx_t e)
{
    seq_t p = (seq_t)1;
    for (idx_t i = (idx_t)0; i < e; ++i) p *= (seq_t)3;
    return p;
}

/* Main sequence function. */
seq_t a(idx_t n)              /* n >= 1 */
{
    idx_t remaining = n;     /* rank within current length, 1‑based */

    /* 1. Determine length L of the desired number. */
    idx_t L = 3;             /* minimal length (222 in base5) */
    while (true) {
        /* Count numbers of length L. */
        seq_t countL = (seq_t)0;

        /* msd = 2 */
        if (L >= 3) {
            seq_t c = binom(L - 1, (idx_t)2);           /* choose 2 of L‑1 positions for 2 */
            c *= pow3(L - 3);                           /* remaining positions free (0,3,4) */
            countL += c;
        }

        /* msd = 3 or 4 */
        if (L >= 4) {
            seq_t c = binom(L - 1, (idx_t)3);           /* choose 3 of L‑1 positions for 2 */
            c *= pow3(L - 4);
            countL += c * (seq_t)2;                     /* two choices for msd (3 or 4) */
        }

        if ((seq_t)remaining <= countL) break;          /* desired length found */

        remaining -= (idx_t)countL;                     /* skip this block */
        ++L;
    }

    /* 2. Build the number digit by digit using combinatorial ranking. */
    seq_t result = (seq_t)0;
    idx_t twos_needed = (idx_t)3;           /* total twos to place */
    idx_t pos_left = L;                     /* positions remaining to fill */

    for (idx_t pos = (idx_t)0; pos < L; ++pos) {
        /* Possible digits in ascending order. */
        const unsigned digits_left[]   = {0, 2, 3, 4};
        const unsigned digits_msd[]    = {2, 3, 4};
        const unsigned *digits;
        idx_t ndigits = 0;

        if (pos == (idx_t)0) {            /* most significant digit cannot be 0 */
            digits = digits_msd;
            ndigits = sizeof(digits_msd)/sizeof(digits_msd[0]);
        } else {
            digits = digits_left;
            ndigits = sizeof(digits_left)/sizeof(digits_left[0]);
        }

        /* Try each candidate digit. */
        for (idx_t d_i = (idx_t)0; d_i < ndigits; ++d_i) {
            unsigned digit = digits[d_i];
            idx_t twos_left = twos_needed - (digit == (unsigned)2 ? (idx_t)1 : (idx_t)0);
            idx_t positions_left = pos_left - (idx_t)1;

            if (twos_left > positions_left) continue;        /* impossible */

            seq_t ways = binom(positions_left, twos_left);
            ways *= pow3(positions_left - twos_left);       /* remaining free positions */

            if ((seq_t)remaining <= ways) {
                /* Digit chosen. */
                result = result * (seq_t)5 + (seq_t)digit;
                twos_needed = twos_left;
                pos_left = positions_left;
                break;
            } else {
                remaining -= (idx_t)ways;                   /* skip those numbers */
            }
        }
    }

    return result;
}
