/*
 * A084173  a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + 2*a(n-4).
 * https://oeis.org/A084173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n+1) - (2*n+1)/2 - (-1)^n/2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+1) using the big‑integer power helper. */
    seq_t power = big_pow((seq_t)2, (seq_t)(n + 1));

    /* Compute the small subtraction term:
       (2*n + 1 + (-1)^n) / 2
       For even n: (2*n + 2)/2 = n + 1
       For odd  n: (2*n) / 2   = n           */
    seq_t small = (seq_t)(2 * n + ((n & 1) ? 0 : 2));
    seq_t sub   = small / (seq_t)2;

    return power - sub;
}
