/*
 * A122801  Number of labeled bipartite graphs on 2n vertices having equal
 *          parts and no isolated vertices.
 * https://oeis.org/A122801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n,n)/2 * Σ_{j=0}^n (-1)^j * C(n,j) * (2^{n-j} - 1)^n
 *            (C(m,k) is binomial coefficient)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   binomial_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts labeled bipartite graphs on 2n vertices with equal
   parts and no isolated vertices. */

#include <stdint.h>
#include "../utils/binomial_simple.h"

/* compute binomial coefficient C(n,k) using seq_t arithmetic */

/* binary exponentiation for seq_t */
static seq_t seq_pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) {
            result = result * base;
        }
        exp >>= 1;
        if (exp) {
            base = base * base;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;   /* a(0) = 1 */

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* Sum over j: number of zero columns to exclude */
    for (idx_t j = 0; j <= n; ++j) {
        /* binomial coefficient C(n,j) */
        seq_t binom = binomial_simple(n, j);

        /* Base 2^{n-j} - 1 */
        seq_t p2 = seq_pow_seq((seq_t)2, n - j);
        seq_t base = p2 - 1;   /* may be 0 when j == n */

        /* If base == 0 and n > 0, term is zero; else compute power */
        seq_t term;
        if (base == 0) {
            term = 0;
        } else {
            term = seq_pow_seq(base, n);   /* (2^{n-j} - 1)^n */
            term = term * binom;           /* multiply by C(n,j) */
        }

        if (j & 1) /* sign (-1)^j is negative when j odd */
            sum_neg += term;
        else
            sum_pos += term;
    }

    /* difference sum_pos - sum_neg (result is guaranteed positive) */
    seq_t sum_fixed;
    if (sum_pos >= sum_neg)
        sum_fixed = sum_pos - sum_neg;
    else
        sum_fixed = sum_neg - sum_pos;   /* should not happen for n>0 */

    /* factor C(2n,n) / 2 */
    seq_t binom_2n_n = binomial_simple(2 * n, n);
    seq_t factor = binom_2n_n / (seq_t)2;  /* integer division, C(2n,n) always even for n>0 */

    /* final result */
    return sum_fixed * factor;
}
