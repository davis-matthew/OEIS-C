/*
 * A206138  G.f.: A(x) = Sum_{n>=0} x^(n*(n+1)/2) / Product_{k=1..n}
 *          (1-x^k)^k.
 * https://oeis.org/A206138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in * Sum_{m>=0} x^(m*(m+1)/2) /
 *            Product_{k=1..m} (1-x^k)^k * * The function computes the values
 *            a(0),…,a(n) by iteratively * building the series for each
 *            partial product and accumulating * the shifted contributions
 * Range:     u8 n <= 17, u16 n <= 36, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Return the n-th term of A206138. */
seq_t a(idx_t n)
{
    /* Convert to a size_t for array indexing.  The values are
     * small enough for the current limits, so this cast is safe. */
    size_t N = (size_t)n;

    /* Allocate arrays of size N+1 and zero‑initialize them
     * by explicit assignment, which works for arbitrary
     * integral types and for big‑integer classes. */
    size_t sz = N + 1;
    seq_t *result = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *cur     = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *next    = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!result || !cur || !next) {
        free(result); free(cur); free(next);
        return 0;  /* allocation failure */
    }
    for (size_t i = 0; i < sz; ++i) result[i] = 0, cur[i] = 0, next[i] = 0;

    cur[0] = 1;  /* f_0(x) = 1 */

    /* Iterate over n_i = 0,1,2,… while the triangular number
     * T = n_i*(n_i+1)/2 does not exceed N. */
    for (size_t n_i = 0;; ++n_i) {
        size_t T = n_i * (n_i + 1) / 2;
        if (T > N) break;

        /* Add the contribution of f_{n_i}(x) shifted by T. */
        for (size_t m = 0; m <= N - T; ++m)
            result[T + m] += cur[m];

        /* Prepare next = cur * (1 - x^{n_i+1})^{-(n_i+1)}.  */
        /* Clear the scratch array. */
        for (size_t i = 0; i < sz; ++i) next[i] = 0;

        size_t k1 = n_i + 1;          /* current factor k */
        seq_t coeff = 1;              /* binomial coefficient for t=0 */

        size_t t = 0;
        while (1) {
            size_t shift = k1 * t;
            if (shift > N) break;
            /* Convolution step */
            for (size_t j = 0; j + shift <= N; ++j) {
                next[j + shift] += cur[j] * coeff;
            }
            ++t;
            if (t > 0) {
                /* coeff = coeff * (k1 + t - 1) / t   (exact division) */
                coeff = coeff * (seq_t)(k1 + t - 1) / (seq_t)(t);
            }
        }

        /* Swap cur and next for the next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Extract the requested term, free the temporary arrays and
     * return the result. */
    seq_t ans = result[N];
    free(result); free(cur); free(next);
    return ans;
}
