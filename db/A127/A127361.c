/*
 * A127361  a(n) = Sum_{k=0..n} binomial(n, floor(k/2))*(-2)^(n-k).
 * https://oeis.org/A127361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, floor(k/2)) * (-2)^(n-k)
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 24, i64 n <= 48, i128 n <= 96,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (C11 + C++17); seq_t and idx_t are defined by the build environment. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

#ifdef __cplusplus
#include <type_traits>
/* Safe multiplication that uses 128 bit intermediate when seq_t is an
   integral type, and falls back to normal multiplication for non‑integral
   seq_t (e.g., an arbitrary‑precision integer class). */
template<typename T>
static inline seq_t safe_mul(T a, T b) {
    if constexpr (std::is_integral<T>::value) {
        return (seq_t)((__int128)a * b);
    } else {
        return a * b;
    }
}
#else
/* For C, seq_t is always an integral type; use __int128 intermediate. */
static inline seq_t safe_mul(seq_t a, seq_t b) {
    return (seq_t)((__int128)a * b);
}
#endif

seq_t a(idx_t n) {
    /* Base cases */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)-1;
    if (n == (idx_t)2) return (seq_t)4;

    /* pre‑compute binomial(n, i) for i = 0 .. floor(n/2) */
    idx_t m = n >> 1;                     /* floor(n/2) */
    seq_t *binom = (seq_t *)malloc((size_t)(m + 1) * sizeof(seq_t));
    if (!binom) return (seq_t)0;          /* allocation failure not expected */

    binom[0] = (seq_t)1;
    for (idx_t i = 1; i <= m; ++i) {
        seq_t prev = binom[i - 1];
        seq_t num  = safe_mul(prev, (seq_t)(n - i + 1)); /* prev * (n-i+1) */
        binom[i]   = num / (seq_t)i;                     /* integer division exact */
    }

    /* power = (-2)^n  (use pow2 and sign adjustment) */
    seq_t abs_pow = pow2(n);           /* 2^n */
    seq_t power = (n & (idx_t)1) ? -abs_pow : abs_pow;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t f = k >> 1;                     /* floor(k/2)   */
        seq_t term = safe_mul(binom[f], power);
        sum += term;
        if (k < n) power = power / (-2);      /* divide by -2 for next power */
    }

    free(binom);
    return sum;
}
