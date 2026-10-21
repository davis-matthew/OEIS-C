/*
 * A099598  Antidiagonal sums of array A099597.
 * https://oeis.org/A099598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*(floor(k/2)!)^2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* pre‑compute factorials up to floor(n/2) */
    idx_t maxF = n / (idx_t)2;
    seq_t *fact = (seq_t *)malloc((maxF + (idx_t)1) * sizeof(seq_t));
    if (!fact) return (seq_t)0;   /* allocation failed – return 0 */
    fact[0] = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= maxF; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* compute the antidiagonal sum */
    seq_t sum = (seq_t)0;
    for (idx_t k = (idx_t)0; k <= n; ++k) {
        seq_t f = fact[k / (idx_t)2];
        sum += binom(n, k) * f * f;
    }

    free(fact);
    return sum;
}
