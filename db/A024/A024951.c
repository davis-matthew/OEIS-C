/*
 * A024951  a(n) = Sum_{i=1..floor((n+2)/4)} a(2i-1)*a(n-2i+1), with
 *          a(1)=a(2)=1 and a(3)=5.
 * https://oeis.org/A024951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..floor((n+2)/4)} a(2i-1) * a(n-2i+1), with
 *            a(1)=1, a(2)=1, a(3)=5. off‑set: 1
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* store a(1) at cache[0] ...    */
static idx_t cache_sz = 0;           /* number of values stored        */
static size_t cache_cap = 0;         /* allocated capacity in elements  */

static void ensure_cache(idx_t n)
{
    if (cache_cap >= n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < (size_t)n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
    if (cache_sz < 3) {              /* initialize the first three terms */
        cache[0] = 1;                /* a(1) */
        cache[1] = 1;                /* a(2) */
        cache[2] = 5;                /* a(3) */
        cache_sz = 3;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behavior below offset is irrelevant */
    if (n <= 3)                      /* base values, no recurrence needed  */
        return (n == 1) ? 1 :
               (n == 2) ? 1 : 5;

    ensure_cache(n);
    if (n <= cache_sz) return cache[n-1];

    for (idx_t m = cache_sz + 1; m <= n; ++m) {
        idx_t k = (m + 2) / 4;       /* floor((m+2)/4) */
        seq_t sum = 0;
        for (idx_t i = 1; i <= k; ++i) {
            idx_t idx1 = 2 * i - 1;  /* 2i-1   */
            idx_t idx2 = m - 2 * i + 1; /* m-2i+1 */
            sum += cache[idx1-1] * cache[idx2-1];
        }
        cache[m-1] = sum;
    }
    cache_sz = n;
    return cache[n-1];
}
