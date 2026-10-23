/*
 * A131046  A007318 * A000203.
 * https://oeis.org/A131046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} binom(n-1,k-1) * σ(k) (A007318, the binomial
 *            transform of σ(n) = sum of divisors)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached divisor sums σ(0..N).  sigma[0] is unused. */
static seq_t *sigma_cache = NULL;
static idx_t   sigma_len   = 0;   /* actual allocated size */

/* Cached values of a(0..N).  a_cache[0] = 0. */
static seq_t *a_cache   = NULL;
static idx_t   a_len     = 0;      /* actual allocated size */

/* Ensure σ(i) is computed for all i ≤ n (inclusive). */
static void ensure_sigma(idx_t n)
{
    if (sigma_len > n) return;           /* already large enough */

    idx_t new_len = n + 1;                /* indices 0..n */
    seq_t *new_sigma = (seq_t*)malloc(new_len * sizeof(seq_t));
    if (!new_sigma) return;              /* allocation failure → abort later */

    /* zero‑initialize */
    for (idx_t i = 0; i < new_len; ++i) new_sigma[i] = 0;

    /* classic divisor‑sum sieve */
    for (idx_t d = 1; d < new_len; ++d) {
        for (idx_t m = d; m < new_len; m += d) {
            new_sigma[m] += (seq_t)d;
        }
    }

    /* replace old cache */
    free(sigma_cache);
    sigma_cache = new_sigma;
    sigma_len   = new_len;
}

/* Ensure a(i) is computed for all i ≤ n (inclusive). */
static void ensure_a(idx_t n)
{
    if (a_len > n) return;               /* already large enough */

    idx_t new_len = n + 1;
    seq_t *new_a = (seq_t*)malloc(new_len * sizeof(seq_t));
    if (!new_a) return;                  /* allocation failure → abort later */

    /* copy previously computed entries (if any) */
    for (idx_t i = 0; i < a_len; ++i) new_a[i] = a_cache[i];

    /* make sure σ is ready up to n */
    ensure_sigma(n);

    /* compute new a values */
    for (idx_t m = a_len; m <= n; ++m) {
        seq_t sum  = 0;
        seq_t coef = 1;                    /* C(m-1,0) */

        for (idx_t k = 1; k <= m; ++k) {
            sum += coef * sigma_cache[k];

            if (k < m) {
                /* next binomial coefficient:
                   C(m-1,k) = C(m-1,k-1) * (m-k) / k   */
                seq_t num = (seq_t)(m - k);
                seq_t den = (seq_t)k;
                coef = (coef * num) / den;
            }
        }
        new_a[m] = sum;
    }

    /* replace old cache */
    free(a_cache);
    a_cache = new_a;
    a_len   = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* the sequence is defined for n≥1 */

    ensure_a(n);
    return a_cache[n];
}
