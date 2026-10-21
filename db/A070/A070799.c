/*
 * A070799  Numbers of the form 6jk-j-k.
 * https://oeis.org/A070799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest positive integer that can be written as
 *            6*j*k - j - k for integers j,k ≥ 1. This is equivalent to all n
 *            for which 6*n+1 has a divisor ≡ 5 (mod 6). The implementation
 *            pre‑computes a large enough block of terms, keeps them sorted,
 *            removes duplicates and answers any a(n) in...
 * Range:     u8 none, u16 none, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function has external linkage and obeys the required signature.  */
#include <stdlib.h>
#include <stdbool.h>

static int cmp_seq(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Return the n-th term of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0 – harmless */
        return 0;

    /* Static cache that is filled only once.  */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* If not yet initialized, generate the sequence up to a safe bound.  */
    if (cache == NULL) {
        /* Choose a bound that guarantees more than 10000 distinct terms
           (JMAX = 400 gives ≈ 80 000 pairs, easily enough).  */
        const idx_t JMAX = 400;
        const size_t est_pairs = JMAX * (JMAX + 1) / 2;
        seq_t *tmp = (seq_t *)malloc(est_pairs * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – return 0 */
        size_t count = 0;

        for (idx_t j = 1; j <= JMAX; ++j) {
            for (idx_t k = j; k <= JMAX; ++k) {   /* ensure j ≤ k to avoid duplicates */
                idx_t val = (idx_t)6 * j * k - j - k;
                tmp[count++] = (seq_t)val;
            }
        }

        /* Sort the values.  */
        qsort(tmp, count, sizeof(seq_t), cmp_seq);

        /* Deduplicate into the final cache.  */
        cache = (seq_t *)malloc(count * sizeof(seq_t));
        if (!cache) { free(tmp); return 0; }
        if (count > 0) {
            cache[0] = tmp[0];
            cache_size = 1;
            for (size_t i = 1; i < count; ++i) {
                if (tmp[i] != tmp[i - 1]) {
                    cache[cache_size++] = tmp[i];
                }
            }
        }
        free(tmp);
    }

    /* If the request exceeds the precomputed size, generate more terms
       by increasing the bound.  (Very unlikely in the test harness.)  */
    while (n > cache_size) {
        /* Double the bound and recompute.  */
        const idx_t JMAX = 800;               /* new bound */
        size_t old_size = cache_size;
        size_t new_cap = old_size * 2 + 10;    /* rough estimate */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return cache[cache_size - 1];
        cache = new_cache;
        cache_size = old_size;                 /* will increase below */

        /* Generate all pairs up to JMAX and append new values.  */
        for (idx_t j = 1; j <= JMAX; ++j) {
            for (idx_t k = j; k <= JMAX; ++k) {
                idx_t val = (idx_t)6 * j * k - j - k;
                if (val <= cache[cache_size - 1]) continue;   /* already in cache */
                cache[cache_size++] = (seq_t)val;
            }
        }

        /* Sort the enlarged array and deduplicate again.  */
        qsort(cache, cache_size, sizeof(seq_t), cmp_seq);
        size_t uniq = 0;
        for (size_t i = 1; i < cache_size; ++i) {
            if (cache[i] != cache[uniq]) {
                cache[++uniq] = cache[i];
            }
        }
        cache_size = uniq + 1;
    }

    return cache[n - 1];
}
