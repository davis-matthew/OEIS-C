/*
 * A055357  Number of increasing mobiles (circular rooted trees) with n nodes
 *          and 3 leaves.
 * https://oeis.org/A055357
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*3^(n-1)/4 - 2^n*(n+1)/2 + n^2/2 + 1/4 (n ≥ 4)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   mul_small, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function accepts the offset 4 as specified by OEIS:  a(4) is the first term. */
#include <stdint.h>
#include "../utils/pow_int.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Ensure we start from the offset; behaviour for n < 4 is unspecified. */
    if (n < 4) return 0;

    /* If seq_t is 64‑bit or smaller, use 128‑bit arithmetic to avoid overflow
       of intermediate terms for the small n that fit in 64 bits (up to about n = 41). */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        /* 3^(n-1) */
        __uint128_t pow3 = 1;
        for (idx_t i = 0; i < n-1; ++i) pow3 *= 3;

        /* 2^n */
        __uint128_t pow2 = 1;
        for (idx_t i = 0; i < n; ++i) pow2 *= 2;

        /* t1 = 5 * 3^(n-1) */
        __uint128_t t1 = 5 * pow3;

        /* t2 = (n+1) * 2^n */
        __uint128_t t2 = (__uint128_t)(n + 1) * pow2;

        /* numerator = t1 - 2*t2 + 2*n^2 + 1 */
        __uint128_t two_t2 = t2 << 1;          /* 2 * t2 */
        __uint128_t n_sq   = (__uint128_t)n * (__uint128_t)n;
        __uint128_t two_nsq = n_sq << 1;       /* 2 * n^2 */

        __uint128_t num = t1 - two_t2 + two_nsq + 1;

        return (seq_t)(num >> 2);             /* divide by 4 */
    }

    /* For larger seq_t (e.g., arbitrary‑precision) compute directly with seq_t ops */
    else {
        /* 3^(n-1) and 2^n in seq_t */
        seq_t pow3 = pow_int(3, n-1);   /* 3^(n-1) */
        seq_t pow2 = pow_int(2, n);     /* 2^n */

        /* t1 = 5 * 3^(n-1) */
        seq_t t1 = mul_small(pow3, 5);

        /* t2 = (n+1) * 2^n */
        seq_t t2 = (seq_t)(n+1) * pow2;

        /* 2*t2 */
        seq_t two_t2 = mul_small(t2, 2);

        /* 2*n^2 */
        seq_t n_sq   = (seq_t)n * (seq_t)n;
        seq_t two_nsq = mul_small(n_sq, 2);

        /* numerator = t1 - 2*t2 + 2*n^2 + 1 */
        seq_t num = t1 - two_t2 + two_nsq + 1;

        return num >> 2; /* divide by 4 */
    }
}
