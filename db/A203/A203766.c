/*
 * A203766  Vandermonde sequence using x^2 + y^2 applied to
 *          (1,1,2,2,...,floor(n/2)).
 * https://oeis.org/A203766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{1<=i<j<=n} (ceil(i/2)^2 + ceil(j/2)^2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the Vandermonde product of the numbers
   1,1,2,2,3,3,…,⌊n/2⌋, where the last value occurs once if n is odd. */

#include "../utils/pow_seq.h"   /* provides seq_t pow_seq(seq_t a, int k) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* outside the defined offset */

    idx_t t = n / 2;               /* number of full value pairs */
    seq_t result = 1;

    /* Handle the first t values (each appearing twice) */
    for (idx_t v = 1; v <= t; ++v)
    {
        /* self‑pair: (2·v²)ⁿC(2,2) = 2·v² once */
        seq_t base = (seq_t)(2 * (seq_t)v * v);
        result *= base;           /* multiply by 2·v² */

        /* pairs with larger values w>v */
        for (idx_t w = v + 1; w <= t; ++w)
        {
            /* base = v² + w², exponent = 4 (two copies of v, two copies of w) */
            seq_t b = (seq_t)((seq_t)v * v + (seq_t)w * w);
            result *= pow_seq(b, 4);
        }
    }

    /* If n is odd, handle the last single value L = t + 1 */
    if (n & 1)
    {
        idx_t L = t + 1;
        for (idx_t v = 1; v <= t; ++v)
        {
            /* base = v² + L², exponent = 2 (two copies of v, one copy of L) */
            seq_t b = (seq_t)((seq_t)v * v + (seq_t)L * L);
            result *= pow_seq(b, 2);
        }
    }

    return result;
}
