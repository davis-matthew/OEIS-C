/*
 * A027411  Number of labeled servers of dimension 24.
 * https://oeis.org/A027411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows from the EGF exp( sum_{i=1}^{24} ((1+x)^i-1)/i ).
 *            This yields the recurrence: a(0) = 1 a(n) = Σ_{k=1}^{min(24,n)}
 *            C(24,k) * a(n-k) * P(n-1,k-1) where C(24,k) is 24 choose k and
 *            P(n-1,k-1) = (n-1)(n-2)…(n-k+1) (empty product = 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of a(0)…a(n) */
static size_t cap = 0;              /* allocated capacity */
static idx_t cached_max = 0;        /* highest n already computed */

/* pre‑compute the binomial coefficients C(24,k) for k=0..24 */
static seq_t C24[25];
static int C24_initialized = 0;

static void init_C24(void) {
    if (C24_initialized) return;
    C24[0] = 1;
    for (idx_t k = 1; k <= 24; ++k) {
        /* C(24,k) = C(24,k-1) * (24-k+1) / k , division is exact */
        C24[k] = C24[k-1] * (seq_t)(24 - k + 1) / (seq_t)k;
    }
    C24_initialized = 1;
}

/* ensure cache can hold values up to n */
static void ensure_cache(idx_t n) {
    if ((size_t)n + 1 <= cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* memory exhaustion – abort */
    cache = tmp;
    for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
    cap = new_cap;
    if (cap > 0) cache[0] = (seq_t)1;  /* a(0) = 1 */
}

/* compute a(n) for any n >= 0 */
seq_t a(idx_t n) {
    init_C24();
    if (n == 0) return (seq_t)1;

    ensure_cache(n);

    /* compute needed terms in ascending order */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        int max_k = (i <= 24) ? (int)i : 24;
        for (int k = 1; k <= max_k; ++k) {
            /* current product P(i-1,k-1) */
            seq_t prod = 1;
            for (int j = 0; j < k-1; ++j) {
                prod *= (seq_t)(i - 1 - j);
            }
            /* term = C(24,k) * a(i-k) * prod */
            seq_t term = C24[k] * cache[i - (idx_t)k];
            term *= prod;
            sum += term;
        }
        cache[i] = sum;
    }
    cached_max = n;
    return cache[n];
}
