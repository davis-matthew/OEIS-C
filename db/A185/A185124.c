/*
 * A185124  Expansion of f(x, -x^5) in powers of x where f(,) is the Ramanujan
 *          general theta function.
 * https://oeis.org/A185124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in Sum_{k∈ℤ} (-1)^⌊(k+1)/2⌋
 *            x^{k(3k+2)}
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of an unsigned __int128, result fits in 64 bits */
static inline unsigned long long isqrt_u128(unsigned __int128 x) {
    unsigned long long low = 0;
    /* sqrt of (1 + 3·(2^64-1)) < 2^33, so this upper bound is safe */
    unsigned long long high = (1ULL << 33) - 1;
    while (low <= high) {
        unsigned long long mid = (low + high) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 == x) return mid;
        if (mid2 < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;  /* floor(sqrt(x)) */
}

seq_t a(idx_t n) {
    /* Compute X = 1 + 3·n, using 128-bit arithmetic to avoid overflow */
    unsigned __int128 X = (unsigned __int128)1 + (unsigned __int128)3 * n;
    unsigned long long t = isqrt_u128(X);

    /* If X is not a perfect square, a(n) = 0 */
    if ((unsigned __int128)t * t != X)
        return 0;

    /* t ≠ 0 mod 3 because t^2 ≡ 1 (mod 3) */
    int t_mod3 = t % 3;
    long long k;
    if (t_mod3 == 1)          /* k = (t-1)/3  (non‑negative) */
        k = (long long)(t - 1) / 3;
    else                      /* t_mod3 == 2, k = -(t+1)/3  (negative) */
        k = -((long long)(t + 1) / 3);

    /* Determine the sign (-1)^{⌊(k+1)/2⌋} using k mod 4 */
    int k_mod4 = (int)((k % 4 + 4) % 4);   /* 0..3 */
    seq_t sign = (k_mod4 == 0 || k_mod4 == 3) ? (seq_t)1 : (seq_t)-1;
    return sign;
}
