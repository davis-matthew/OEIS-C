/*
 * A187947  Positions of 0 in A187946; complement of A134862.
 * https://oeis.org/A187947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A187947
 *  Positions of 0 in A187946; complement of A134862.
 *
 *  The sequence consists of all positive integers k such that
 *      floor((k+5)*φ) = floor(k*φ) + 8   ,   φ = (1+√5)/2
 *
 *  Using the identity
 *      floor(k*φ) = (k + floor(√(5*k*k))) / 2
 *  we can compute the condition using only integer arithmetic.
 */

#include <stdint.h>

/* integer square‑root of a 64‑bit unsigned value */
static inline uint64_t isqrt_u64(uint64_t x) {
    uint64_t lo = 0, hi = (uint64_t)1 << 32;          /* sqrt(2^64‑1) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) lo = mid + 1;
        else hi = mid;
    }
    return lo - 1;
}

/* floor(k * φ) for k >= 0 using only integer arithmetic */
static inline uint64_t floor_kphi(uint64_t k) {
    __uint128_t prod = (__uint128_t)5 * k * k;   /* 5*k*k  fits in 128‑bit */
    uint64_t v   = (uint64_t)prod;
    uint64_t sq  = isqrt_u64(v);                 /* floor(sqrt(5*k*k)) */
    return (k + sq) / 2;
}

/* required OEIS function */
seq_t a(idx_t n) {
    idx_t found = 0;
    idx_t k = 1;

    while (found < n) {
        /* floor(k*φ) and floor((k+5)*φ) */
        uint64_t ak     = floor_kphi((uint64_t)k);
        uint64_t ak5    = floor_kphi((uint64_t)(k + 5));

        if (ak5 - ak == 8) {          /* condition is satisfied */
            ++found;
            if (found == n)
                return (seq_t)k;     /* the k‑th element */
        }
        ++k;
    }
    return 0;                           /* never reached */
}
