/*
 * A211148  Number of 2 X 2 matrices having all terms in {-n,...,0,..,n} and
 *          positive determinant.
 * https://oeis.org/A211148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n+1)^4 - (24n^2 + 8n + 1) - 16 * Σ_{k=2}^n φ(k) *
 *            floor(n/k)^2) / 2 where φ(k) is Euler's totient function
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   compute_phi_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A211148: number of 2x2 matrices with entries in [-n..n] having
   positive determinant. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_phi_table.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* sequence defined for n>=1 */

    uint64_t *phi = compute_phi_table(n);
    if (!phi) return 0;    /* memory failure – unlikely for small n */

    seq_t sum_phi_floor = 0;
    for (idx_t k = 2; k <= n; ++k) {
        idx_t t = n / k;           /* floor(n/k) */
        seq_t t_sq = (seq_t)t * (seq_t)t;
        sum_phi_floor += (seq_t)phi[k] * t_sq;
    }

    seq_t M   = (seq_t)n * 2 + 1;  /* = 2n + 1, always odd */
    seq_t M2  = M * M;
    seq_t M4  = M2 * M2;          /* (2n+1)^4 */

    seq_t inner = (seq_t)24 * (seq_t)n * (seq_t)n + (seq_t)8 * (seq_t)n + 1;
    seq_t term1_num = M4 - inner;   /* even number */
    seq_t term1 = term1_num / 2;    /* first part of formula */

    seq_t result = term1 - (seq_t)8 * sum_phi_floor;

    free(phi);
    return result;
}
