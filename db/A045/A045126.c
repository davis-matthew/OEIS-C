/*
 * A045126  Numbers whose base-4 representation contains exactly three 1's and
 *          two 3's.
 * https://oeis.org/A045126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth smallest integer whose base‑4 representation
 *            contains exactly three 1's and two 3's (the other digits are 0
 *            or 2)
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Find the n-th number with exactly three 1's and two 3's in its base‑4 representation. */
seq_t a(idx_t n)
{
    /* Find the length L of the base‑4 representation. */
    idx_t L = 5;                 /* minimal length (3+2 digits). */
    seq_t cumulative = 0;        /* sum of counts for lengths < L. */
    seq_t countL;

    for (;; ++L)
    {
        countL = binomial(L, 3) * binomial(L - 3, 2) * pow2(L - 5);
        if (cumulative + countL >= n)
            break;
        cumulative += countL;
    }

    /* Rank within the numbers of length L (1‑based). */
    seq_t k = (seq_t)n - cumulative;

    /* Digit construction. */
    seq_t      result = 0;
    idx_t      rem1 = 3;   /* remaining 1's to place. */
    idx_t      rem3 = 2;   /* remaining 3's to place. */

    for (idx_t pos = 0; pos < L; ++pos)
    {
        /* Remaining positions after this one. */
        idx_t left = L - pos - 1;

        /* Iterate over possible digits in increasing order. */
        for (int d = 0; d <= 3; ++d)
        {
            if (pos == 0 && d == 0)          /* leading zero not allowed */
                continue;

            idx_t newRem1 = rem1;
            idx_t newRem3 = rem3;

            if (d == 1) {
                if (newRem1 == 0) continue;   /* cannot use more 1's than needed */
                --newRem1;
            } else if (d == 3) {
                if (newRem3 == 0) continue;   /* cannot use more 3's than needed */
                --newRem3;
            }

            /* Check feasibility. */
            if (newRem1 > left || newRem3 > left || newRem1 + newRem3 > left)
                continue;

            /* Count completions for this choice. */
            seq_t ways = binomial(left, newRem1);
            ways *= binomial(left - newRem1, newRem3);
            ways *= pow2(left - newRem1 - newRem3);

            if (k <= ways) {
                /* Pick this digit. */
                result = result * 4 + d;
                rem1 = newRem1;
                rem3 = newRem3;
                break;
            }
            k -= ways;  /* skip these many numbers */
        }
    }

    return result;
}
