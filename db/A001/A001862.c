/*
 * A001862  Number of forests of least height with n nodes.
 * https://oeis.org/A001862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. exp(x*(exp(x)-x/2)). Differentiating gives a(n+1) =
 *            Sum_{k=0..n} binomial(n,k)*c(k)*a(n-k), a(0) = 1, where c(0) =
 *            1, c(1) = 1, c(k) = k+1 for k >= 2 (c(k) are the e.g.f.
 *            coefficients of exp(x) + x*exp(x) - x)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A_memo = NULL;   /* A_memo[i] = a(i), i < A_len */
static seq_t *R_row = NULL;    /* R_row[k] = binomial(A_len-1, k) */
static idx_t A_len = 0;
static idx_t A_cap = 0;

static void grow_to(idx_t need) {
    idx_t newcap = A_cap ? A_cap : 16;
    idx_t i;
    seq_t *na, *nr;
    while (newcap < need) newcap *= 2;
    na = (seq_t *)calloc((size_t)newcap, sizeof(seq_t));
    nr = (seq_t *)calloc((size_t)newcap, sizeof(seq_t));
    for (i = 0; i < A_len; i++) {
        na[i] = A_memo[i];
        nr[i] = R_row[i];
    }
    if (A_memo) free(A_memo);
    if (R_row) free(R_row);
    A_memo = na;
    R_row = nr;
    A_cap = newcap;
}

seq_t a(idx_t n) {
    if (n < A_len) return A_memo[n];
    if (n + 1 > A_cap) grow_to(n + 1);
    if (A_len == 0) {
        A_memo[0] = 1;
        R_row[0] = 1;
        A_len = 1;
    }
    while (A_len <= n) {
        idx_t i = A_len - 1;
        idx_t k;
        seq_t sum = 0;
        for (k = 0; k <= i; k++) {
            seq_t c = (k < 2) ? (seq_t)1 : ((seq_t)k + 1);
            sum += R_row[k] * c * A_memo[i - k];
        }
        A_memo[A_len] = sum;
        /* advance Pascal row from i to i+1 */
        R_row[i + 1] = 1;
        for (k = i; k >= 1; k--) {
            R_row[k] += R_row[k - 1];
        }
        A_len++;
    }
    return A_memo[n];
}
