/*
 * A003224  The number of superpositions of cycles of order n of the groups
 *          E_3 and D_n.
 * https://oeis.org/A003224
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/(2n)) * Sum_{g in D_n} F(g)^2, where F(g) is the
 *            number of undirected Hamiltonian cycles on [n] invariant under
 *            g, and F(g) = N(g)*z(g)/(2n) (N(g) = number of elements of D_n
 *            of g's cycle type, z(g) = its centralizer size in S_n). Cycle
 *            types: identity (F=(n-1)!/2); rotations of order d...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 18 (2026-11-01)
 * Helpers:   totient_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/totient_idx.h"

/* adds M*F^2/(2n) as quotient Q plus remainder R (mod two) */
static void a3224_add(seq_t *Q, uint64_t *R, idx_t M, seq_t F, idx_t two) {
    seq_t qF = F / two;
    uint64_t rF = (uint64_t)(F % two);
    uint64_t t;
    *Q += (seq_t)M * (qF * qF * two + qF * (rF * 2));
    t = (uint64_t)M * rF * rF;
    *Q += (seq_t)(t / two);
    *R += t % two;
}

/* c * d^e * m! / 2, halving the first even factor to keep intermediates small */
static seq_t a3224_prod_half(idx_t c, idx_t d, idx_t e, idx_t m) {
    int pending = 1;
    idx_t x = c, i;
    seq_t r;
    if (x % 2 == 0) { x /= 2; pending = 0; }
    r = (seq_t)x;
    for (i = 0; i < e; i++) {
        idx_t f = d;
        if (pending && f % 2 == 0) { f /= 2; pending = 0; }
        r = r * f;
    }
    for (i = 2; i <= m; i++) {
        idx_t f = i;
        if (pending && f % 2 == 0) { f /= 2; pending = 0; }
        r = r * f;
    }
    if (pending) r = r / 2;
    return r;
}

seq_t a(idx_t n) {
    idx_t two, d, k;
    seq_t Q = 0;
    uint64_t R = 0;
    if (n < 3) return 0;
    two = 2 * n;

    /* identity */
    a3224_add(&Q, &R, 1, a3224_prod_half(1, 1, 0, n - 1), two);

    /* rotations (and edge-reflections merged into type 2^(n/2)) */
    for (d = 2; d <= n; d++) {
        if (n % d == 0) {
            idx_t m = n / d;
            idx_t N = totient_idx(d);
            if (d == 2) N += m;
            a3224_add(&Q, &R, N, a3224_prod_half(N, d, m - 1, m - 1), two);
        }
    }

    /* vertex reflections */
    k = (n - 1) / 2;
    a3224_add(&Q, &R, (n % 2 == 1) ? n : n / 2, a3224_prod_half(1, 2, k, k), two);

    Q += (seq_t)(R / two);
    return Q;
}
