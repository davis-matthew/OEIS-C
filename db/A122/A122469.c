/*
 * A122469  Write Roman numerals I, V, X, L, C, D, M in increasing order: [I],
 *          [I][V], [I][V][X], ..., etc.
 * https://oeis.org/A122469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = concat_{k=1}^{n} ((k%2==1 ? 1 : 5) * 10^{(k-1)/2})
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Local cache for already computed terms */
static seq_t *cache    = NULL;
static idx_t  cache_cap = 0;      /* allocated capacity in terms */
static idx_t  cache_size = 0;     /* number of terms currently stored */

/* Ensure that cache can hold at least 'n' elements */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;
    idx_t new_cap = cache_cap ? (cache_cap << 1) : 1;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* offset is 1, so n=0 is undefined */

    /* Return cached value if already known */
    if (n <= cache_size) return cache[n-1];

    /* Prepare for new computations */
    ensure_cache(n);
    idx_t  start   = cache_size + 1;               /* first index to compute */
    idx_t  m_start = (start - 1) / 2;              /* exponent for pow10 at start */
    seq_t  pow10   = (seq_t)1;                     /* pow10 = 10^m_start */
    for (idx_t i = 0; i < m_start; ++i) pow10 *= (seq_t)10;

    seq_t r = (cache_size > 0) ? cache[cache_size-1] : (seq_t)0;  /* a(start-1) */

    /* Compute terms from start to n */
    for (idx_t k = start; k <= n; ++k) {
        seq_t term = (k & 1) ? pow10           : pow10 * (seq_t)5;
        seq_t pow10_d = pow10 * (seq_t)10;      /* 10^(m+1) */

        r = r * pow10_d + term;
        cache[k-1] = r;                         /* store a(k) */

        /* Advance pow10 for the next index */
        if ((k & 1) == 0) pow10 *= (seq_t)10;   /* exponent increases after even k */
    }

    cache_size = n;
    return r;
}
