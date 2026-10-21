/*
 * A053284  Coefficients of the '10th-order' mock theta function chi(q).
 * https://oeis.org/A053284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of q^n in Sum_{m>=0} (-1)^m q^{(m+1)^2} /
 *            ((1+q)(1+q^2)...(1+q^{2m+1}))
 * Range:     i8 n <= 61, i16 n <= 195, i32 n <= 663, i64 n <= 2374,
 *            i128 n <= 8844, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes the sequence by a direct
   dynamic–programming convolution.  For each m the series
   S(q) = 1/((1+q)(1+q^2)...(1+q^{2m+1})) is maintained up
   to the requested index N.  The coefficient of q^t in S is
   found in array S[t].  The contribution of this m to
   a(n) is obtained by shifting S by (m+1)^2 and adding
   (-1)^m*S[t] to a[t+(m+1)^2].  After the contribution is
   added, S is updated to 1/((1+q)…(1+q^{2m+3})) by
   multiplying with 1/(1+q^{2m+2}) and 1/(1+q^{2m+3})
   using the recurrence
           new[j] = old[j] - new[j-k]   (j>=k)   or   new[j]=old[j]   (j<k).  */

#include <stdlib.h>

/* Static cache: a_array holds a(0)...a(max_cache)
   and cache_size is the largest computed index. */
static seq_t *a_array = NULL;
static idx_t cache_size = 0;

/* Compute coefficients up to N (inclusive).  The function
   returns the pointer to a_array and updates cache_size. */
static void compute_up_to(idx_t N)
{
    /* Release old cache if larger */
    if (a_array) {
        free(a_array);
        a_array = NULL;
    }

    /* Allocate result array and temporary buffers */
    seq_t *A = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *S = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *nextS = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!A || !S || !nextS) {
        /* In case of failure, leave cache empty and return */
        free(A); free(S); free(nextS);
        a_array = NULL;
        cache_size = 0;
        return;
    }

    /* Initialise S as 1/(1+q) = 1 - q + q^2 - q^3 + ... */
    for (idx_t j = 0; j <= N; ++j)
        S[j] = (j & 1) ? -1 : 1;

    /* Main loop over m */
    for (idx_t m = 0;; ++m) {
        idx_t shift = (idx_t)((m + 1) * (m + 1));
        if (shift > N) break;                /* no further contributions */

        seq_t sign = (m & 1) ? -1 : 1;        /* (-1)^m */

        /* Add contribution to A for all t such that t+shift <= N */
        for (idx_t t = 0; t + shift <= N; ++t) {
            A[t + shift] += sign * S[t];
        }

        /* Update S by multiplying with 1/(1+q^{2m+2}) */
        idx_t k = 2 * m + 2;
        if (k <= N) {
            for (idx_t j = 0; j <= N; ++j) {
                if (j < k)
                    nextS[j] = S[j];
                else
                    nextS[j] = S[j] - nextS[j - k];
            }
            /* Swap S and nextS */
            seq_t *tmp = S; S = nextS; nextS = tmp;
        }

        /* Update S by multiplying with 1/(1+q^{2m+3}) */
        k = 2 * m + 3;
        if (k <= N) {
            for (idx_t j = 0; j <= N; ++j) {
                if (j < k)
                    nextS[j] = S[j];
                else
                    nextS[j] = S[j] - nextS[j - k];
            }
            /* Swap S and nextS */
            seq_t *tmp = S; S = nextS; nextS = tmp;
        }
    }

    /* Free temporary buffers and store result in cache */
    free(S); free(nextS);
    a_array = A;
    cache_size = N;
}

/* Public function required by the build.  It guarantees
   that a(n) is computed correctly for any n >= 0 and
   caches intermediate results for efficiency. */
seq_t a(idx_t n)
{
    if (cache_size < n) {
        /* Recompute sequence up to n */
        compute_up_to(n);
    }
    /* If computation failed, a_array may be NULL; treat as 0 */
    if (!a_array) return (seq_t)0;
    return a_array[n];
}
