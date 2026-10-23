/*
 * A103821  A Whitney transform of the central binomial coefficients A000984.
 * https://oeis.org/A103821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=11; for n≥3, a(n) = ((5n-2)*a(n-1) -
 *            2*a(n-2) - 4*(n-1)*a(n-3)) / n (recurrence found in the OEIS
 *            entry)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>               /* for malloc, realloc, abort */

static seq_t *a_cache = NULL;     /* memoised values a(0)..a(len-1) */
static idx_t a_cache_len = 0;     /* number of cached entries */

/* Ensure base values are present */
static void a_ensure_base(void)
{
    if (a_cache_len != 0) return;
    a_cache_len = 3;
    a_cache = (seq_t *)malloc(a_cache_len * sizeof(seq_t));
    if (!a_cache) abort();
    a_cache[0] = (seq_t)1;
    a_cache[1] = (seq_t)3;
    a_cache[2] = (seq_t)11;
}

/* Compute and cache values up to index n (inclusive) */
static void a_ensure_upto(idx_t n)
{
    a_ensure_base();
    if (a_cache_len > n) return;          /* already computed */

    /* Grow the cache */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, (size_t)new_len * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;

    /* Compute missing terms */
    for (idx_t i = a_cache_len; i <= n; ++i) {
        if (i == 0) { a_cache[0] = (seq_t)1; continue; }
        if (i == 1) { a_cache[1] = (seq_t)3; continue; }
        if (i == 2) { a_cache[2] = (seq_t)11; continue; }

        /* (5*i - 2) * a[i-1] */
        seq_t term1 = ((seq_t)(5 * i - 2)) * a_cache[i - 1];
        /* 2 * a[i-2] */
        seq_t term2 = (seq_t)2 * a_cache[i - 2];
        /* 4*(i-1) * a[i-3] */
        seq_t term3 = (seq_t)4 * (seq_t)(i - 1) * a_cache[i - 3];

        seq_t numer = term1 - term2 - term3;
        a_cache[i] = numer / (seq_t)i;
    }
    a_cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* Return cached term if already computed */
    if (a_cache_len > n && a_cache[n] != 0) {
        return a_cache[n];
    }

    /* Ensure all needed terms are computed */
    a_ensure_upto(n);
    return a_cache[n];
}
