/*
 * A177199  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=8, k=-1 and l=1.
 * https://oeis.org/A177199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=8; a(n+1)=∑_{p=0}^{n}(a(p)*a(n-p)-1)+1
 *            (equivalently: a(n+1)=∑_{p=0}^{n}a(p)a(n-p)-n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */

static seq_t *cache = NULL;       /* array of computed terms */
static size_t cache_cap = 0;      /* allocated capacity (in terms) */
static size_t cache_len = 0;      /* number of terms that have been computed */

/* Ensure the cache can hold at least (n+1) terms. */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap;
    if (new_cap == 0)
        new_cap = 2;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a contest environment we simply abort on OOM. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) according to the recurrence. */
seq_t a(idx_t n)
{
    /* Initialise the cache if this is the first call. */
    if (cache_len == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache_len = 2;
    }

    /* If the requested term is already computed, return it. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    ensure_capacity(n);

    /* Compute missing terms up to n. */
    for (idx_t m = cache_len; m <= n; ++m) {
        /* a(m) = sum_{p=0}^{m-1} a(p)*a(m-1-p) - (m-1)  */
        seq_t sum = (seq_t)0;
        for (idx_t p = 0; p < m; ++p) {
            idx_t q = (m - 1) - p;
            sum += cache[(size_t)p] * cache[(size_t)q];
        }
        sum -= (seq_t)(m - 1);
        cache[(size_t)m] = sum;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
