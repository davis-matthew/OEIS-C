/*
 * A151684  Let c(n) = x^(2^n-1)*(1-x^(2^n)), g(n) = 1 + x^(2^n-1) + x^(2^n),
 *          h(n) = Product_{i=1..n} g(i); then use g.f. (1+2*x) - Sum_{n>=1}
 *          c(n)/h(n).
 * https://oeis.org/A151684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 39, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A151684
   a(n) is the coefficient of x^n in the generating function
   F(x) = (1+2x) - Σ_{k≥1} c(k)/h(k)
   where c(k)=x^(2^k-1)*(1-x^(2^k)),
         g(k)=1+x^(2^k-1)+x^(2^k),
         h(k)=∏_{i=1..k} g(i)
   The function returns the n‑th term of the sequence.
*/

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;    /* series coefficients up to cache_len-1 */
static idx_t  cache_len = 0;   /* number of terms stored */

static void compute_series(idx_t N, seq_t *F)
{
    /* F[0..N] will contain the series coefficients a(0)..a(N)   */
    seq_t *h      = (seq_t *)calloc(N+1, sizeof(seq_t));
    seq_t *newh   = (seq_t *)calloc(N+1, sizeof(seq_t));
    seq_t *inv    = (seq_t *)calloc(N+1, sizeof(seq_t));
    idx_t  k, pow, e1, e2, i, j, m, maxdeg;

    /* initialize base series */
    h[0] = 1;

    /* determine maximal k needed: 2^k <= N+1 */
    k = 0;
    while (k < 63 && ((uint64_t)1 << (k+1)) <= (uint64_t)(N + 1))
        ++k;
    for (idx_t n = 1; n <= (idx_t)k; ++n) {
        pow = (uint64_t)1 << n;                    /* 2^n   */

        /* -------- build g(n) --------------- */
        seq_t *g = (seq_t *)calloc(N+1, sizeof(seq_t));
        g[0] = 1;
        if (pow - 1 <= (uint64_t)N) g[pow-1] += 1;
        if (pow <= (uint64_t)N)     g[pow]   += 1;

        /* -------- multiply h by g (mod x^(N+1)) ------------- */
        for (i = 0; i <= N; ++i) {
            if (h[i] == 0) continue;
            maxdeg = N - i;
            for (j = 0; j <= maxdeg; ++j) {
                if (g[j] == 0) continue;
                newh[i+j] += h[i] * g[j];
            }
        }
        /* copy newh into h and reset newh */
        for (i = 0; i <= N; ++i) { h[i] = newh[i]; newh[i] = 0; }

        /* -------- compute inverse of h ------------- */
        inv[0] = 1;
        for (m = 1; m <= N; ++m) {
            seq_t sum = 0;
            /* only terms up to m where h[k] != 0 */
            for (i = 1; i <= m; ++i) {
                if (h[i] == 0) continue;
                sum += h[i] * inv[m - i];
            }
            inv[m] = -sum;
        }

        /* -------- add contribution -c(n)/h(n) ------------- */
        /* c(n) has two non‑zero terms: +x^(pow-1)  and  -x^(2*pow-1) */
        e1 = pow - 1;
        e2 = 2 * pow - 1;
        /* first term: +x^(pow-1)  (coeff 1) */
        if ((uint64_t)e1 <= (uint64_t)N) {
            idx_t maxlen = N - e1;
            for (i = 0; i <= maxlen; ++i) {
                F[e1 + i] -= inv[i];        /* subtract because of overall minus */
            }
        }
        /* second term: -x^(2*pow-1)  (coeff -1) */
        if ((uint64_t)e2 <= (uint64_t)N) {
            idx_t maxlen = N - e2;
            for (i = 0; i <= maxlen; ++i) {
                F[e2 + i] += inv[i];        /* minus of minus */
            }
        }

        free(g);
    }

    /* finally add the base series (1 + 2x) */
    F[0] += 1;
    if (N >= 1) F[1] += 2;

    free(h);
    free(newh);
    free(inv);
}

seq_t a(idx_t n)
{
    if (n >= cache_len) {
        /* allocate or enlarge cache */
        seq_t *new_cache = (seq_t *)calloc(n+1, sizeof(seq_t));
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_len = n+1;

        /* compute series up to n */
        compute_series(n, cache);
    }
    return cache[n];
}
