/*
 * A005039  Number of nonequivalent dissections of a polygon into n pentagons
 *          by nonintersecting diagonals rooted at a cell up to rotation and
 *          reflection.
 * https://oeis.org/A005039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: (1/10)*x*(u^5(x) + 4*u(x^5) + 5*u^2(x^2) + 5*x*u^4(x^2)),
 *            u(x) = g.f. of A002293, using [x^m] u(x)^k = k/(4m+k) *
 *            binomial(4m+k, m). So a(n) = ( [x^(n-1)]u^5 + 4*[x^((n-1)/5)]u +
 *            5*[x^((n-1)/2)]u^2 + 5*[x^((n-2)/2)]u^4 ) / 10, where a term is
 *            dropped when its index is not an integer
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"

static unsigned __int128 g128(unsigned __int128 x, unsigned __int128 y) {
    while (y) { unsigned __int128 t = x % y; x = y; y = t; }
    return x;
}

/* coef: f_k(m) = k/(4m+k)*C(4m+k,m), returned as q = f/10, r = f%10.
   Step: f(i+1) = f(i) * N / D with N/D in lowest terms, so D divides f(i). */
#define DEF_ALL(SUF, T)                                                        \
static void coef_##SUF(unsigned k, idx_t m, T *qo, unsigned *ro) {             \
    T q = 0; unsigned r = 1;                                                   \
    for (idx_t i = 0; i < m; i++) {                                            \
        unsigned __int128 b = 4 * (unsigned __int128)i + k;                    \
        unsigned __int128 c = 3 * (unsigned __int128)i + k;                    \
        unsigned __int128 N = b * (b + 1) * (b + 2) * (b + 3);                 \
        unsigned __int128 D = ((unsigned __int128)i + 1) * (c + 1) * (c + 2) * (c + 3); \
        unsigned __int128 g = g128(N, D);                                      \
        N /= g; D /= g;                                                        \
        uint64_t Nn = (uint64_t)N, Dn = (uint64_t)D;                           \
        T q1 = q / (idx_t)Dn;                                                  \
        uint64_t q0 = (uint64_t)(long long)(q % (idx_t)Dn);                    \
        unsigned __int128 t = (unsigned __int128)q0 * 10 + r;                  \
        unsigned __int128 s = t / Dn;                                          \
        unsigned __int128 sn = s * Nn;                                         \
        q = q1 * (idx_t)Nn + (idx_t)(uint64_t)(sn / 10);                       \
        r = (unsigned)(sn % 10);                                               \
    }                                                                          \
    *qo = q; *ro = r;                                                          \
}                                                                              \
static T total_##SUF(idx_t n) {                                                \
    T q, acc; unsigned r, rs;                                                  \
    coef_##SUF(5, n - 1, &q, &r);                                              \
    acc = q; rs = r;                                                           \
    if ((n - 1) % 5 == 0) {                                                    \
        coef_##SUF(1, (n - 1) / 5, &q, &r);                                    \
        acc += q * 4; rs += 4 * r;                                             \
    }                                                                          \
    if ((n - 1) % 2 == 0) {                                                    \
        coef_##SUF(2, (n - 1) / 2, &q, &r);                                    \
        acc += q * 5; rs += 5 * r;                                             \
    }                                                                          \
    if (n >= 2 && n % 2 == 0) {                                                \
        coef_##SUF(4, (n - 2) / 2, &q, &r);                                    \
        acc += q * 5; rs += 5 * r;                                             \
    }                                                                          \
    acc += (idx_t)(rs / 10);                                                   \
    return acc;                                                                \
}

DEF_ALL(seq, seq_t)
DEF_ALL(w128, unsigned __int128)

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (seq_is_fixed_width() && sizeof(seq_t) < 16)
        return u128_to_seq(total_w128(n));
    return total_seq(n);
}
