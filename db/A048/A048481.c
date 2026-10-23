/*
 * A048481  a(n) = T(0,n) + T(1,n-1) + ... + T(n,0), array T given by A048472.
 * https://oeis.org/A048481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) * (n-2) + 2*n + 5, a(0)=1, a(1)=3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the two small cases directly, avoiding a negative cast. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* 2^(n+1) */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n + 1));

    /* (n-2) is non‑negative for n >= 2, so we can cast safely. */
    seq_t mult = pow2 * (seq_t)(n - 2);

    /* The remaining linear part: 2*n + 5. */
    seq_t add = (seq_t)(2 * n + 5);

    return mult + add;
}
