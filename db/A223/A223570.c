/*
 * A223570  Number of n X n 0..2 arrays with antidiagonals unimodal.
 * https://oeis.org/A223570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = U(n) * ∏_{i=1}^{n‑1} U(i)^2 , where U(x) = (x^4 + 6*x^3 +
 *            23*x^2 + 18*x + 24) / 24 (OEIS A223570)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of partial products:  P[0] = 1,
       P[k] = ∏_{i=1}^{k} U(i)  for k ≥ 1  */
    static seq_t *P = NULL;
    static size_t P_cap = 0;   /* allocated size in elements */
    static idx_t max_n = 0;    /* highest index for which P[i] is valid */

    if (n == 0) return (seq_t)0;   /* undefined for the offset 1 */

    /* we need indices 0 … n‑1 inclusive → n elements               */
    size_t need = (size_t)n;

    /* allocate / grow the array if necessary */
    if (P == NULL) {
        P = (seq_t *)malloc(need * sizeof(seq_t));
        if (!P) return (seq_t)0;     /* allocation failure */
        P_cap = need;
    } else if (need > P_cap) {
        seq_t *tmp = (seq_t *)realloc(P, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        P = tmp;
        P_cap = need;
    }

    /* initialise the empty product and zero‑th entry once */
    if (max_n == 0 && P_cap > 0)
        P[0] = (seq_t)1;

    /* extend the table up to index n‑1 if not already done   */
    if (max_n < n - 1) {
        for (idx_t i = max_n + 1; i <= n - 1; ++i) {
            seq_t Ui = ((seq_t)i * (seq_t)i * (seq_t)i * (seq_t)i
                       + 6 * (seq_t)i * (seq_t)i * (seq_t)i
                       + 23 * (seq_t)i * (seq_t)i
                       + 18 * (seq_t)i
                       + 24) / 24;
            P[i] = P[i - 1] * Ui;
        }
        max_n = n - 1;
    }

    /* compute U(n) */
    seq_t U_n = ((seq_t)n * (seq_t)n * (seq_t)n * (seq_t)n
                 + 6 * (seq_t)n * (seq_t)n * (seq_t)n
                 + 23 * (seq_t)n * (seq_t)n
                 + 18 * (seq_t)n
                 + 24) / 24;

    /* product of U(i)^2 for i = 1 … n‑1  =  (∏ U(i))^2  */
    seq_t prod = P[n - 1];
    seq_t prod_sq = prod * prod;

    return U_n * prod_sq;
}
