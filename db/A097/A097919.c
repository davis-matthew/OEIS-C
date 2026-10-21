/*
 * A097919  a(1)=1; a(n+1) = Sum_{k=1 to n} a(k) a(ceiling(n/k)).
 * https://oeis.org/A097919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = Sum_{k=1}^{n} a(k) * a(ceil(n/k))
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* static cache of computed terms   */
static size_t cache_cap = 0;      /* current capacity of seq_cache   */
static idx_t  cache_max = 0;      /* largest n for which a(n) is cached */

/*---------------------------------------------------------------*/
/*  OEIS sequence A097919                                         */
/*  a(1) = 1                                                      */
/*  a(n+1) = Sum_{k=1}^{n} a(k) * a(ceil(n/k))  (n ≥ 1)           */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;                            /* offset is 1 */

    /* ensure cache has room for indices up to n          */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                         /* OOM – abort (unlikely) */
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the base case if not done yet             */
    if (cache_max == 0) {
        seq_cache[1] = (seq_t)1;
        cache_max   = 1;
    }

    /* compute any missing terms up to n                    */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t sum = 0;
        idx_t limit = m - 1;                       /* n-1 for this term */
        for (idx_t k = 1; k <= limit; ++k) {
            idx_t idx2 = (limit + k - 1) / k;      /* ceil(limit/k)    */
            sum = sum + seq_cache[k] * seq_cache[idx2];
        }
        seq_cache[m] = sum;
    }

    cache_max = n;
    return seq_cache[n];
}
