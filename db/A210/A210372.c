/*
 * A210372  Number of 2 X 2 matrices with all elements in {0,1,...,n} and
 *          positive even determinant.
 * https://oeis.org/A210372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∑_{p even} f(p)² + ∑_{p odd} f(p)² – ∑_{a,b,c,d}
 *            1_{ad=bc}) / 2 where f(p) = #pairs (x,y) with x·y = p, 0≤x,y≤n.
 *            This is derived from counting 2×2 matrices with positive even
 *            determinant
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 340, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_table_build
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/phi_table_build.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* basic combinatorial counts */
    idx_t odd_cnt = (n + 1) / 2;             /* #odd numbers in [0,n] */
    seq_t tot_total = (seq_t)(n + 1) * (seq_t)(n + 1);          /* (n+1)²                                         */
    seq_t tot_odd  = (seq_t)odd_cnt * (seq_t)odd_cnt;          /* only products of two odds are odd                */
    seq_t tot_even = tot_total - tot_odd;                     /* remaining products are even                       */

    /* count of zero‑determinant matrices (ad = bc = 0) */
    seq_t two_n1 = (seq_t)(2 * n + 1);
    seq_t zero_cnt = two_n1 * two_n1;

    /* tot_quad = Σ f(p)² = #solutions to ad = bc        */
    /*   = zero_cnt  +  Σ_{g=1..n} count_coprime(g) * ⌊n/g⌋²   */
    uint32_t *phi = phi_table_build(n);          /* phi[1..n] available */

    seq_t nonzero_sum = 0;
    for (idx_t g = 1; g <= n; ++g) {
        uint32_t c = (g == 1) ? 1 : 2 * phi[g]; /* number of coprime pairs with max = g */
        idx_t t = n / g;
        seq_t t_sq = (seq_t)t * (seq_t)t;
        nonzero_sum += (seq_t)c * t_sq;
    }
    free(phi);

    seq_t tot_quad = zero_cnt + nonzero_sum;

    /* final formula */
    seq_t term = (tot_even * tot_even + tot_odd * tot_odd - tot_quad) / 2;
    return term;
}
