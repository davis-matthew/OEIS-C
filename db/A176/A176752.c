/*
 * A176752  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=0, k=0 and l=-2.
 * https://oeis.org/A176752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 23, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176752
   a(0) = 1, a(1) = 0
   a(n+1) = Σ_{p=0..n} a(p)*a(n-p) - 2   (n ≥ 1)
   (OEIS recurrence with k = 0, l = -2) */

#include <stdlib.h>  /* malloc, realloc */

/* Cached values of the sequence */
static seq_t *cache = NULL;
static idx_t  cap   = 0;   /* allocated capacity (number of slots) */
static idx_t  len   = 0;   /* number of values stored (indices 0..len-1) */

/* Resize the cache to hold indices up to `needed` (inclusive) */
static void ensure_cache(idx_t needed)
{
    /* Initialise on first use */
    if (len == 0) {
        cap = (idx_t)2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();          /* out‑of‑memory abort */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        len = 2;
    }

    if (needed < len) return;          /* already computed */

    /* Grow capacity if necessary */
    if (needed >= cap) {
        idx_t newcap = cap ? cap << 1 : (idx_t)2;
        while (needed >= newcap)
            newcap = newcap << 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* Compute new terms from previous length up to `needed` */
    for (idx_t i = len; i <= needed; ++i) {
        idx_t m = i - 1;                  /* convolution index   */
        seq_t sum = (seq_t)0;
        for (idx_t p = 0; p <= m; ++p)    /* Σ a[p] * a[m-p] */
            sum += cache[p] * cache[m - p];
        cache[i] = sum - (seq_t)2;        /* recurrence: conv - 2 */
    }
    len = needed + 1;
}

/* Return the nth term of the sequence (n ≥ 0) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
