/*
 * A007451  Denominators of expansion of exp x / sin x.
 * https://oeis.org/A007451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of [x^n] exp(x)/sin(x) = denominator of
 *            g(n+1), where x*exp(x)/sin(x) = Sum g(j) x^j. With Q = product
 *            of odd primes <= n+2 and H(j) = j!*Q*g(j) (an integer):
 *            (j+1)*H(j) = (j+1)*Q - Sum_{i>=1}
 *            (-1)^i*binomial(j+1,2i+1)*H(j-2i), H(0) = Q. Then a(n) =
 *            (n+1)!*Q / gcd(H(n+1), (n+1)...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 10, u64 n <= 21, u128 n <= 27,
 *            big any n
 * Helpers:   a7451_isp, alloc_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/a7451_isp.h"

typedef unsigned __int128 a7451_u128;

static a7451_u128 *a7451_alloc128(idx_t len) {
    return (a7451_u128 *)calloc(len, sizeof(a7451_u128));
}

static void a7451_free128(a7451_u128 *v, idx_t len) {
    (void)len;
    free(v);
}

#define A7451_GEN(NAME, T, ALLOC, FREE)                                   \
static T NAME(idx_t n) {                                                  \
    idx_t J = n + 1;                                                      \
    T Q = 1;                                                              \
    for (idx_t p = 3; p <= J + 1; p += 2)                                 \
        if (a7451_isp(p)) Q *= p;                                         \
    T *H = ALLOC(J + 1);                                                  \
    T *row = ALLOC(J + 3);                                                \
    row[0] = 1;                                                           \
    row[1] = 1;                                                           \
    for (idx_t j = 0; j <= J; j++) {                                      \
        T pos = Q;                                                        \
        pos *= (j + 1);                                                   \
        T neg = 0;                                                        \
        for (idx_t i = 1; 2 * i <= j; i++) {                              \
            T t = row[2 * i + 1] * H[j - 2 * i];                          \
            if (i & 1) pos += t; else neg += t;                           \
        }                                                                 \
        H[j] = (pos - neg) / (j + 1);                                     \
        for (idx_t k = j + 2; k >= 1; k--) row[k] += row[k - 1];          \
    }                                                                     \
    T M = Q;                                                              \
    for (idx_t k = 2; k <= J; k++) M *= k;                                \
    T x = H[J], y = M;                                                    \
    while (y != 0) { T t = x % y; x = y; y = t; }                         \
    T res = M / x;                                                        \
    FREE(H, J + 1);                                                       \
    FREE(row, J + 3);                                                     \
    return res;                                                           \
}

A7451_GEN(a7451_seq, seq_t, alloc_zero, release)
A7451_GEN(a7451_wide, a7451_u128, a7451_alloc128, a7451_free128)

seq_t a(idx_t n) {
    if (seq_is_big()) return a7451_seq(n);
    return (seq_t)a7451_wide(n);
}
