/*
 * A127943  a(n) = 2^binomial(n+1,2)/A046161(n).
 * https://oeis.org/A127943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n*(n-3)/2 + popcount(n))
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* popcount of n (number of 1 bits) */
    idx_t pop = 0;
    idx_t t = n;
    while (t) {
        t &= t - 1;
        pop++;
    }

    /* intermediate computation in signed 64‑bit to avoid underflow */
    long long nn   = (long long)n;
    long long tmp  = (nn * nn - 3 * nn) / 2;   /* n*(n-3)/2 */
    idx_t exponent = (idx_t)((long long)pop + tmp); /* non‑negative */

    return pow2(exponent);
}
