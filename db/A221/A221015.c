/*
 * A221015  The hyper-Wiener index of the dendrimer G_n , defined pictorially
 *          in the Ashrafi - Shabani - Diudea reference.
 * https://oeis.org/A221015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3171 + 2^(n-1)*(-810*n^2 +3240*n -35583) +
 *            4^(n-1)*(11664*n^2 -38232*n+58482)
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Use signed arithmetic to evaluate the polynomial part. */
    long long nn = (long long)n;
    long long t1 = -810LL * nn * nn + 3240LL * nn - 35583LL;
    long long t2 = 11664LL * nn * nn - 38232LL * nn + 58482LL;

    /* Compute powers of 2 and 4. */
    seq_t pow2n = pow_int((seq_t)2, n > 0 ? n - 1 : 0);
    seq_t pow4n = pow_int((seq_t)4, n > 0 ? n - 1 : 0);

    /* Absolute value of the first polynomial coefficient. */
    seq_t abs_t1 = (t1 < 0) ? (seq_t)(-t1) : (seq_t)t1;

    /* Multiply the terms. */
    seq_t part1 = pow2n * abs_t1;          /* 2^(n-1) * |t1| */
    seq_t part2 = pow4n * (seq_t)t2;       /* 4^(n-1) * t2  */

    seq_t result = 3171 + part2;           /* start with constant + second part */

    if (t1 < 0)
        result -= part1;                  /* subtract when t1 is negative */
    else
        result += part1;                  /* add when t1 is positive */

    return result;
}
