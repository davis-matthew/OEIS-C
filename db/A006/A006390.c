/*
 * A006390  Number of sensed loopless planar maps with n edges.
 * https://oeis.org/A006390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2n))*[2(4n+1)*binomial(4n, n)/((n+1)*(3n+1)*(3n+2)) +
 *            Sum_{0<k<n, k|n} phi(n/k)*binomial(4k, k) + q(n)], q(n) =
 *            binomial(2n, (n-2)/2) if n even, 2n*binomial(2n, (n-1)/2)/(n+1)
 *            if n odd; a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 25, u128 n <= 45,
 *            big any n
 * Helpers:   phi_ull, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/phi_ull.h"
#include "../utils/u128_to_seq.h"
#include "../utils/seq_is_fixed_width.h"

#define DEFINE_CALC(NAME, T)                                                  \
static T NAME##_binom(uint64_t m, uint64_t k) {                               \
    if (k > m) return (T)0;                                                   \
    if (k > m - k) k = m - k;                                                 \
    T r = (T)1;                                                               \
    for (uint64_t i = 0; i < k; i++) {                                        \
        r = r * (T)(m - i);                                                   \
        r = r / (T)(i + 1);                                                   \
    }                                                                         \
    return r;                                                                 \
}                                                                             \
static T NAME(idx_t n) {                                                      \
    T nn = (T)n;                                                              \
    /* 2(4n+1)C(4n,n)/((n+1)(3n+1)(3n+2)) = 2*C(4n+1,n)/((n+1)(3n+2)) */     \
    T first = (T)2 * NAME##_binom(4 * n + 1, n);                              \
    first = first / ((nn + 1) * (3 * nn + 2));                                \
    T sum = (T)0;                                                             \
    for (idx_t k = 1; k < n; k++) {                                           \
        if (n % k == 0) {                                                     \
            T ph = (T)phi_ull((unsigned long long)(n / k));                   \
            sum += ph * NAME##_binom(4 * k, k);                               \
        }                                                                     \
    }                                                                         \
    T q;                                                                      \
    if (n % 2 == 0) {                                                         \
        q = NAME##_binom(2 * n, (n - 2) / 2);                                 \
    } else {                                                                  \
        q = (T)2 * nn * NAME##_binom(2 * n, (n - 1) / 2);                     \
        q = q / (nn + 1);                                                     \
    }                                                                         \
    T total = first + sum + q;                                                \
    return total / ((T)2 * nn);                                               \
}

DEFINE_CALC(calc_wide, unsigned __int128)
DEFINE_CALC(calc_seq, seq_t)

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (seq_is_fixed_width() && sizeof(seq_t) <= 8)
        return u128_to_seq(calc_wide(n));
    return calc_seq(n);
}
