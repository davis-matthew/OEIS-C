/*
 * A214971  Integers k for which the base-phi representation of k includes 1.
 * https://oeis.org/A214971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((n-1)*φ) + 2*n – 1, φ = (1+√5)/2
 * Range:     u8 n <= 71, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root of a 128‑bit value via binary search.
   Returns floor(sqrt(x)).                               */
static inline uint64_t isqrt_u128(__uint128_t x)
{
    uint64_t r = 0;
    /* start with the largest power of two ≤ sqrt(max_uint128).  */
    uint64_t bit = 1ULL << 62;   /* (2^62)^2 = 2^124 < 2^128 */

    while (bit) {
        uint64_t next = r + bit;
        __uint128_t sq   = (__uint128_t)next * next;
        if (sq <= x) r = next;
        bit >>= 1;
    }
    return r;
}

/* Compute a(n) for A214971.                                            */
seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence.  */
    uint64_t k = (uint64_t)(n - 1);                /* k = n-1 */

    /* floor(k*√5) == floor(sqrt(5*k*k))                  */
    __uint128_t prod = (__uint128_t)5 * k * k;
    uint64_t floor_k_sqrt5 = isqrt_u128(prod);

    /* floor(k*φ) = (k + floor(k*√5)) / 2                 */
    uint64_t floor_k_phi = (k + floor_k_sqrt5) / 2;

    /* final value: floor((n-1)*φ) + 2*n – 1               */
    uint64_t res = floor_k_phi + 2 * n - 1;
    return (seq_t)res;
}
