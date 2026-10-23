/*
 * A118207  Expansion of Product_{k>=1} (1 + x^k)^lambda(k) where lambda(k) is
 *          the Liouville function, A008836.
 * https://oeis.org/A118207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + x^k)^λ(k), where λ(k)
 *            is the Liouville function. The algorithm constructs the
 *            generating function incrementally
 * Range:     i8 n <= 49, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Utility: smallest‑prime‑factor sieve ---------- */
static void build_spf(idx_t limit, uint64_t **out)
{
    /* Allocate and initialise the array. */
    uint64_t *spf = (uint64_t *)calloc((size_t)limit + 1, sizeof(uint64_t));
    if (!spf) { *out = 0; return; }

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;                    /* i is prime */
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    *out = spf;
}

/* -------------------- Main function --------------------- */
seq_t a(idx_t n)
{
    /* Allocate arrays for coefficients. */
    seq_t   *A = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t   *B = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!A || !B) { free(A); free(B); return 0; }

    A[0] = (seq_t)1;            /* constant term */

    /* Special case n = 0.  Nothing else to do. */
    if (n == 0) {
        seq_t res = A[0];
        free(A); free(B);
        return res;
    }

    /* Compute Liouville λ(k) for k = 1 … n. */
    uint64_t *spf = 0;
    build_spf(n, &spf);
    if (!spf) { free(A); free(B); return 0; }

    int8_t *lam = (int8_t *)malloc((size_t)n + 1);
    if (!lam) { free(A); free(B); free(spf); return 0; }

    lam[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        lam[i] = (int8_t)(-lam[i / spf[i]]);
    }

    /* Expand the product term by term. */
    for (idx_t k = 1; k <= n; ++k) {
        if (lam[k] == 1) {                   /* multiply by (1 + x^k) */
            for (idx_t i = n; i >= k; --i) {
                A[i] += A[i - k];
            }
        } else {                              /* multiply by (1 - x^k + x^{2k} - …) */
            for (idx_t i = 0; i <= n; ++i) {
                B[i] = A[i];
                if (i >= k) {
                    B[i] -= B[i - k];
                }
            }
            /* swap A ↔ B */
            seq_t *tmp = A;
            A = B;
            B = tmp;
        }
    }

    seq_t result = A[n];

    /* Clean up */
    free(A);
    free(B);
    free(spf);
    free(lam);

    return result;
}
