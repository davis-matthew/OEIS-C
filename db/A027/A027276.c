/*
 * A027276  a(n) = Sum_{k=0..2n} (k+1) * A026552(n, k).
 * https://oeis.org/A027276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 93,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A027276
   a(0) = 1;
   For even n = 2m (m ≥ 1): a(n) = (3/2) * 6^m * (2m + 1)
   For odd  n = 2m + 1 : a(n) = 6^(m + 1) * (m + 1)
   These formulas arise from the closed form given in OEIS. */
#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 separately. */
    if (n == 0)
        return 1;

    /* m = floor(n/2) */
    idx_t m = n >> 1;

    if ((n & 1) == 0)   /* Even n : n = 2m,  m ≥ 1 */
    {
        /* Compute 6^m */
        seq_t pow6_m = big_pow(6, (seq_t)m);

        /* \frac{3}{2} * 6^m = 3 * 6^m / 2, division is exact for m ≥ 1 */
        seq_t half  = pow6_m >> 1;                /* 6^m / 2         */
        seq_t prod  = mul_small(half, 3);         /* 3 * 6^m / 2     */
        prod        = mul_small(prod, (unsigned int)(2 * m + 1)); /* * (2m+1) */

        return prod;
    }
    else                /* Odd n : n = 2m + 1 */
    {
        /* Compute 6^m */
        seq_t pow6_m = big_pow(6, (seq_t)m);

        /* 6^(m+1) = 6 * 6^m */
        seq_t prod = mul_small(pow6_m, 6);

        /* multiply by (m + 1) */
        prod = mul_small(prod, (unsigned int)(m + 1));

        return prod;
    }
}
