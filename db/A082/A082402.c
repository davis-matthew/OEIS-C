/*
 * A082402  Number of n-node labeled weakly connected acyclic digraphs.
 * https://oeis.org/A082402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003024(n) - Sum_{k=1..n-1} binomial(n-1,
 *            k-1)*a(k)*A003024(n-k) with a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* static data for memoisation */
    static seq_t *D = NULL;          /* A003024 values */
    static seq_t *A = NULL;          /* desired values */
    static idx_t capacity = 0;       /* current array size   */
    static idx_t last = 0;           /* highest n computed   */

    /* initialise arrays if first call */
    if (capacity == 0) {
        capacity = 1;
        D = (seq_t *)malloc(sizeof(seq_t));
        A = (seq_t *)malloc(sizeof(seq_t));
        if (!D || !A) return (seq_t)0;   /* allocation failure */
        D[0] = (seq_t)1;                  /* one acyclic digraph on 0 vertices */
        A[0] = (seq_t)0;                  /* a(0) by definition */
    }

    /* grow arrays if necessary */
    if (n + 1 > capacity) {
        idx_t new_cap = capacity * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *D2 = (seq_t *)realloc(D, new_cap * sizeof(seq_t));
        seq_t *A2 = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!D2 || !A2) {
            free(D2);
            free(A2);
            return (seq_t)0;
        }
        D = D2;
        A = A2;
        capacity = new_cap;
    }

    /* compute values that are not yet known */
    for (idx_t m = last + 1; m <= n; ++m) {
        /* ---- compute D[m] : number of labelled acyclic digraphs ---- */
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t bin = binomial(m, k);
            idx_t exp = k * (m - k);
            seq_t pow = pow2(exp);
            seq_t term = bin * pow * D[m - k];
            if (k & 1U)
                pos += term;
            else
                neg += term;
        }
        D[m] = pos - neg;   /* guaranteed non‑negative */

        /* ---- compute a[m] using the recurrence --------------------- */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m - 1; ++k) {
            seq_t term = binomial(m - 1, k - 1) * A[k] * D[m - k];
            sum += term;
        }
        A[m] = D[m] - sum;
    }
    last = n;
    return A[n];
}
