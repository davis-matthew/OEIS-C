/*
 * A027446  Triangle read by rows: square of the lower triangular mean matrix.
 * https://oeis.org/A027446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = numerator of the (n-th) entry of the square of the lower
 *            triangular mean matrix, read by rows. Derivation: For row i,
 *            column j (1 ≤ j ≤ i) A^2[i,j] = (1/i) * Σ_{k=j}^i 1/k The least
 *            common denominator of row i is D_i = i * lcm(1..i). The
 *            numerator of A^2[i,j] is num(i,j) = (lcm(1..i) / lcm...
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

/* Helper: lcm of two seq_t values */
static seq_t _lcm_seq_t(seq_t a, seq_t b)
{
    return (a / gcd(a, b)) * b;
}

/* Find the row index k such that k*(k+1)/2 ≥ n */
static idx_t _row_for_idx(idx_t n)
{
    idx_t k = 1;
    while (k * (k + 1) / 2 < n)
        ++k;
    return k;
}

/* Compute the sequence value a(n) */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1 is defined, but handle gracefully */
        return 0;

    /* Determine row k and column j */
    idx_t k = _row_for_idx(n);
    idx_t j = n - (k - 1) * k / 2;   /* 1‑based column within row k */

    /* Allocate space for Lrange[1..k] */
    seq_t *Lrange = (seq_t *)calloc((size_t)(k + 1), sizeof(seq_t));
    if (!Lrange)
        return 0;          /* allocation failure – should never occur */

    seq_t L = 1;          /* running lcm of the processed suffix */
    for (idx_t jj = k; jj >= 1; --jj) {
        seq_t jj_seq = (seq_t)jj;
        L = _lcm_seq_t(L, jj_seq);   /* lcm of numbers jj..k */
        Lrange[jj] = L;
    }
    seq_t L_i = Lrange[1];           /* lcm(1..k) */

    /* Compute the numerator sum Σ_{k=j}^k Lrange[j]/k */
    seq_t sum = 0;
    seq_t L_j = Lrange[j];
    for (idx_t kk = j; kk <= k; ++kk) {
        seq_t kk_seq = (seq_t)kk;
        sum += L_j / kk_seq;     /* exact division */
    }

    /* Factor L_i / L_j is integer */
    seq_t factor = L_i / L_j;
    seq_t result = factor * sum;

    free(Lrange);
    return result;
}
