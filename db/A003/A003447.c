/*
 * A003447  Number of nonequivalent dissections of an n-gon into n-3 polygons
 *          by nonintersecting diagonals rooted at a cell up to rotation and
 *          reflection.
 * https://oeis.org/A003447
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = S(n)/(2n), counted with Burnside's lemma over the
 *            dihedral group, where S(n) = (n-3)*C(2n-5,n-4) + [n even] (
 *            m*C_{m-1}/2 + m*(C_{m-1} + C_{m-2} + 3*T3(m-3)) ), m = n/2 +
 *            [4|n] 2*q*C_{q-1}, q = n/4 + [n odd] 2n*C_{(n-1)/2-1}, C_k =
 *            Catalan(k), T3(k) = [x^k] C(x)^3 = 3/(2k+3)*binomial(2k+3,...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 36, u128 n <= 67,
 *            big any n
 * Helpers:   a3447_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3447_gcd.h"

#define A3447_DEF(NAME, T)                                                   \
static T NAME##_binom(uint64_t N, uint64_t K) {                              \
    T c = 1;                                                                 \
    uint64_t i;                                                              \
    for (i = 1; i <= K; i++) {                                               \
        uint64_t num = N - K + i;                                            \
        uint64_t r = (uint64_t)(long long)(c % (T)i);                        \
        uint64_t g = a3447_gcd(r, i);                                        \
        c = (c / (T)g) * (T)(num / (i / g));                                 \
    }                                                                        \
    return c;                                                                \
}                                                                            \
static T NAME##_cat(uint64_t k) {                                            \
    return NAME##_binom(2 * k, k) / (T)(k + 1);                              \
}                                                                            \
static T NAME##_val(uint64_t n) {                                            \
    T S = (T)(n - 3) * NAME##_binom(2 * n - 5, n - 4);                       \
    if (n % 2 == 0) {                                                        \
        uint64_t m = n / 2;                                                  \
        T r;                                                                 \
        S += ((T)m * NAME##_cat(m - 1)) / (T)2;                              \
        if (n % 4 == 0) {                                                    \
            uint64_t q = n / 4;                                              \
            S += (T)(2 * q) * NAME##_cat(q - 1);                             \
        }                                                                    \
        r = NAME##_cat(m - 1) + NAME##_cat(m - 2);                           \
        if (m >= 3) {                                                        \
            uint64_t k = m - 3;                                              \
            r += (T)3 * ((NAME##_binom(2 * k + 3, k) * (T)3) / (T)(2 * k + 3)); \
        }                                                                    \
        S += (T)m * r;                                                       \
    } else {                                                                 \
        uint64_t H = (n - 1) / 2;                                            \
        S += (T)(2 * n) * NAME##_cat(H - 1);                                 \
    }                                                                        \
    return S / (T)(2 * n);                                                   \
}

A3447_DEF(a3447w, unsigned __int128)
A3447_DEF(a3447s, seq_t)

seq_t a(idx_t n) {
    if (n <= 60) {
        unsigned __int128 v = a3447w_val((uint64_t)n);
        uint64_t hi = (uint64_t)(v >> 64);
        uint64_t lo = (uint64_t)v;
        seq_t r = (seq_t)lo;
        if (hi) r += ((seq_t)hi << 32) << 32;
        return r;
    }
    return a3447s_val((uint64_t)n);
}
