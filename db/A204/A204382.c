/*
 * A204382  G.f.: Product_{n>=1} (1 - A002203(n)*x^n + (-1)^n*x^(2*n)) where
 *          A002203(n) is the companion Pell numbers.
 * https://oeis.org/A204382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 – A002203(k)·x^k +
 *            (–1)^k ·x^(2k)) where A002203(k) are the Pell–Lucas numbers: L_0
 *            = 2, L_1 = 2, L_k = 2·L_{k‑1} + L_{k‑2} (k≥2)
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 25, i64 n <= 50, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation recomputes the coefficient in O(n²) time.
   The index n is 0‑based, so a(0)=1.  The function works for
   all integer types supplied as seq_t (including arbitrary‑precision). */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) { seq_t one = (seq_t)1; return one; }

    /* Allocate and compute Pell‑Lucas numbers up to n. */
#ifdef __cplusplus
    seq_t *lucas = new seq_t[n+1]();      /* value‑initialized to zero */
#else
    seq_t *lucas = (seq_t*)calloc((n+1), sizeof(seq_t));
#endif
    lucas[0] = (seq_t)2;
    lucas[1] = (seq_t)2;
    for (idx_t i = 2; i <= n; ++i) {
        lucas[i] = (seq_t)((seq_t)2 * lucas[i-1] + lucas[i-2]);
    }

    /* Allocate coefficient arrays. */
#ifdef __cplusplus
    seq_t *prev = new seq_t[n+1]();
    seq_t *curr = new seq_t[n+1]();
#else
    seq_t *prev = (seq_t*)calloc((n+1), sizeof(seq_t));
    seq_t *curr = (seq_t*)calloc((n+1), sizeof(seq_t));
#endif
    prev[0] = (seq_t)1;   /* start with polynomial 1 */

    /* Apply factors one by one. */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t sign = (k & 1) ? (seq_t)(-1) : (seq_t)1;
        seq_t lk   = lucas[k];

        for (idx_t i = 0; i <= n; ++i) {
            seq_t val = prev[i];
            if (i >= k)      val -= lk * prev[i-k];
            if (i >= 2*k)    val += sign * prev[i-2*k];
            curr[i] = val;
        }
        /* swap prev ↔ curr for next iteration */
        seq_t *tmp = prev; prev = curr; curr = tmp;
    }

    seq_t result = prev[n];

    /* Free allocated memory. */
#ifdef __cplusplus
    delete[] prev;
    delete[] curr;
    delete[] lucas;
#else
    free(prev);
    free(curr);
    free(lucas);
#endif

    return result;
}
