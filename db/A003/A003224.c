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
 * Helpers:   a3224_add, a3224_phi, a3224_prod_half
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a3224_add.h"
#include "../utils/a3224_phi.h"
#include "../utils/a3224_prod_half.h"

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
            idx_t N = a3224_phi(d);
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
