/*
 * A159435  Number of n X n arrays of squares of integers summing to 35.
 * https://oeis.org/A159435
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of z^35 in (1 + z + z^4 + z^9 + z^16 +
 *            z^25)^{n^2}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts n×n arrays of integer squares whose sum equals 35. */

#include <stdint.h>

static seq_t binom_small(idx_t n, int r)
{
    seq_t res = 1;
    for (int i = 1; i <= r; ++i)
    {
        res = (res * (n - i + 1)) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* values undefined below the offset */

    idx_t k = n * n;   /* number of cells in the array */

    seq_t total = 0;

    /* Enumerate all solutions to 25*a25 + 16*a16 + 9*a9 + 4*a4 + a1 = 35 */
    for (int a25 = 0; 25 * a25 <= 35; ++a25)
    {
        int rem25 = 35 - 25 * a25;
        for (int a16 = 0; 16 * a16 <= rem25; ++a16)
        {
            int rem16 = rem25 - 16 * a16;
            for (int a9 = 0; 9 * a9 <= rem16; ++a9)
            {
                int rem9 = rem16 - 9 * a9;
                for (int a4 = 0; 4 * a4 <= rem9; ++a4)
                {
                    int a1 = rem9 - 4 * a4;   /* 1*a1 fills the remaining sum */
                    int sum_nonzero = a25 + a16 + a9 + a4 + a1;
                    if (sum_nonzero > k) /* not enough positions for non‑zeros */
                        continue;

                    idx_t rem = k;
                    seq_t coeff = 1;

                    if (a25 > 0)
                    {
                        coeff *= binom_small(rem, a25);
                        rem -= a25;
                    }
                    if (a16 > 0)
                    {
                        coeff *= binom_small(rem, a16);
                        rem -= a16;
                    }
                    if (a9 > 0)
                    {
                        coeff *= binom_small(rem, a9);
                        rem -= a9;
                    }
                    if (a4 > 0)
                    {
                        coeff *= binom_small(rem, a4);
                        rem -= a4;
                    }
                    if (a1 > 0)
                    {
                        coeff *= binom_small(rem, a1);
                        rem -= a1;
                    }
                    /* rem now equals a0 (number of zero squares) */
                    total += coeff;
                }
            }
        }
    }
    return total;
}
