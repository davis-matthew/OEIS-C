/*
 * A007474  Number of circular chord diagrams with n chords, up to rotational
 *          symmetry.
 * https://oeis.org/A007474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Burnside over the rotations of the 2n points: a(n) = (1/(2n)) *
 *            Sum_{d | 2n} phi(d) * F(d), a(0) = 1, a(1) = 0, where with m =
 *            2n/d, F(d) = Sum_{j=0..floor(m/2)} (-1)^j * M(m,j) * T(m-2j),
 *            M(m,j) = m/(m-j)*binomial(m-j,j) (j-matchings of the m-cycle of
 *            adjacent-edge orbits), T(r) = [d even]*T(r-1...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   a7474_phi, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a7474_phi.h"

#define A7474_DEF_F(NAME, T)                                              \
static T NAME(idx_t n, idx_t d) {                                         \
    idx_t m = 2 * n / d;                                                  \
    int ev = (d % 2 == 0);                                                \
    idx_t jmax = m / 2;                                                   \
    idx_t r = m - 2 * jmax;                                               \
    T tp, tc;                                                             \
    if (r == 0) { tp = 0; tc = 1; }                                       \
    else { tp = 1; tc = ev ? (T)1 : (T)0; }                               \
    T M;                                                                  \
    if (m == 1) M = 1;                                                    \
    else if (m % 2 == 0) M = 2;                                           \
    else M = (T)m;                                                        \
    T pos = 0, neg = 0;                                                   \
    for (idx_t j = jmax;; j--) {                                          \
        T term = M * tc;                                                  \
        if (j % 2) neg += term; else pos += term;                         \
        if (j == 0) break;                                                \
        M = M * (T)j * (T)(m - j) / ((T)(m - 2 * j + 2) * (T)(m - 2 * j + 1)); \
        for (int s = 0; s < 2; s++) {                                     \
            T tn = (ev ? tc : (T)0) + (T)r * (T)d * tp;                   \
            tp = tc;                                                      \
            tc = tn;                                                      \
            r++;                                                          \
        }                                                                 \
    }                                                                     \
    return pos - neg;                                                     \
}

A7474_DEF_F(a7474_F_big, seq_t)
A7474_DEF_F(a7474_F_fix, unsigned __int128)

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    idx_t N = 2 * n;
    if (seq_is_fixed_width()) {
        unsigned __int128 S = 0;
        for (idx_t d = 1; d <= N; d++)
            if (N % d == 0)
                S += (unsigned __int128)a7474_phi(d) * a7474_F_fix(n, d);
        return u128_to_seq(S / (unsigned __int128)N);
    }
    seq_t S = 0;
    for (idx_t d = 1; d <= N; d++)
        if (N % d == 0)
            S += (seq_t)a7474_phi(d) * a7474_F_big(n, d);
    return S / (seq_t)N;
}
