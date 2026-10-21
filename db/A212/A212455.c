/*
 * A212455  a(n) = floor(2n*log(2n)).
 * https://oeis.org/A212455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(2*n*log(2*n))
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ln2_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ln2_fixed.h"

/* helper: floor(log2(n)) for n>=1 */
static inline idx_t floor_log2_u64(idx_t x) {
    /* GCC/Clang intrinsic provides fastest path */
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll(x);
#else
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
#endif
}

seq_t a(idx_t n) {
    /* constants */
    const unsigned long long SCALE = ((unsigned long long)1) << 40;   /* 2^40 */
    const unsigned long long ln2fixed = ln2_fixed();                 /* ln(2)*SCALE floor */

    /* get binary exponent of n */
    idx_t k = floor_log2_u64(n);
    unsigned long long pow2k = (((unsigned long long)1) << k);

    /* A = n - 2^k, B = n + 2^k  (both fit in 64 bits) */
    unsigned long long A = n > pow2k ? n - pow2k : 0;
    unsigned long long B = n + pow2k;

    /* x = (n - 2^k) / (n + 2^k)  in fixed-point (scale 2^40) */
    unsigned long long x_fixed = ((unsigned __int128)A << 40) / B;      /* floor division */

    /* fixed point of x^2 */
    unsigned long long x_squared_fixed = ((unsigned __int128)x_fixed * x_fixed) >> 40;

    /* compute ln(m) where m = n / 2^k  (m ∈ [1,2)) using atanh series */
    unsigned long long term = x_fixed;   /* current odd power of x */
    unsigned long long sum = 0;          /* fixed-point sum of x^(2i+1)/(2i+1) */

    /* terms sufficient for < 2.5e-5 absolute error on ln(m) (n ≤ 10000) */
    for (int i = 0; i < 10 && term; ++i) {
        unsigned long long denom = (unsigned long long)2 * i + 1;
        sum += term / denom;            /* floor division keeps fixed-point */
        /* next odd power: term = term * x_squared_fixed >> 40 */
        term = ((unsigned __int128)term * x_squared_fixed) >> 40;
    }
    unsigned long long ln_m_fixed = sum * 2ULL;   /* ln(m) scaled by 2^40 */

    /* ln(2*n) = ln(m) + (k+1)*ln(2) */
    unsigned long long ln2n_fixed = ln_m_fixed + (k + 1ULL) * ln2fixed;

    /* compute floor(2*n*ln(2*n)) using 128-bit intermediate to avoid overflow */
    __uint128_t prod = (__uint128_t)(2ULL * (unsigned long long)n) * (__uint128_t)ln2n_fixed;
    unsigned long long result = (unsigned long long)(prod >> 40);   /* floor division by SCALE */

    return (seq_t)result;
}
