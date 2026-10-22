/*
 * A001376  Relational systems on n nodes.
 * https://oeis.org/A001376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1*s_1+2*s_2+...=n} fix
 *            A[s_1,s_2,...]/(1^s_1*s_1!*2^s_2*s_2!*...), fix A[s_1,s_2,...] =
 *            4^Sum_{i,j,k>=1} (i*j*k*s_i*s_j*s_k/lcm(i,j,k)). Each term
 *            2^(2e)/denom is split as q + r/d' with denom = 2^v*d' (d' odd),
 *            and the remainders are summed over the common odd denominator D
 *            = odd part...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   r_absorb, r_gcd, r_lcm, r_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/r_absorb.h"
#include "../utils/r_gcd.h"
#include "../utils/r_lcm.h"
#include "../utils/r_pow2.h"

typedef struct {
    idx_t n;
    seq_t D;      /* odd part of n! */
    seq_t Q;      /* sum of integer quotients */
    seq_t R;      /* sum of remainders scaled to denominator D */
    idx_t *parts;
    idx_t *mult;
} r_state;

static void r_leaf(r_state *st, int k) {
    uint64_t e = 0;
    for (int x = 0; x < k; x++)
        for (int y = 0; y < k; y++)
            for (int z = 0; z < k; z++) {
                uint64_t i = st->parts[x], j = st->parts[y], l = st->parts[z];
                uint64_t L = r_lcm(r_lcm(i, j), l);
                e += (i * j * l / L) * st->mult[x] * st->mult[y] * st->mult[z];
            }
    seq_t dodd = 1;
    uint64_t v = 0;
    for (int x = 0; x < k; x++) {
        for (idx_t c = 0; c < st->mult[x]; c++) r_absorb(st->parts[x], &dodd, &v);
        for (idx_t c = 2; c <= st->mult[x]; c++) r_absorb(c, &dodd, &v);
    }
    seq_t num = r_pow2(2 * e - v);
    seq_t q = num / dodd;
    seq_t r = num % dodd;
    st->Q += q;
    st->R += r * (st->D / dodd);
}

static void r_rec(r_state *st, idx_t rem, idx_t maxp, int k) {
    if (rem == 0) { r_leaf(st, k); return; }
    idx_t top = maxp < rem ? maxp : rem;
    for (idx_t p = top; p >= 1; p--) {
        for (idx_t m = 1; m * p <= rem; m++) {
            st->parts[k] = p;
            st->mult[k] = m;
            r_rec(st, rem - m * p, p - 1, k + 1);
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    r_state st;
    st.n = n;
    st.D = 1;
    uint64_t vv = 0;
    for (idx_t i = 2; i <= n; i++) r_absorb(i, &st.D, &vv);
    st.Q = 0;
    st.R = 0;
    st.parts = (idx_t *)malloc((n + 2) * sizeof(idx_t));
    st.mult = (idx_t *)malloc((n + 2) * sizeof(idx_t));
    r_rec(&st, n, n, 0);
    free(st.parts);
    free(st.mult);
    return st.Q + st.R / st.D;
}
