/*
 * A144370  Shifts 6 places left under Dirichlet convolution.
 * https://oeis.org/A144370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(n) = sum_{i*j = n-6} a(i)*a(j) for n>6 Initial values:
 *            a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=1
 * Range:     u8 n <= 41, u16 n <= 87, u32 n <= 183, u64 n <= 375,
 *            u128 n <= 759, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* 1‑based cache for computed terms      */
static idx_t  max_computed = 0;   /* largest index for which cache[i] is valid */

/* Ensure cache has space for indices 0..n (indices 0 is unused)   */
static void ensure_cache(idx_t n)
{
    if (cache && max_computed >= n) return;          /* already large enough */
    size_t new_size = (size_t)n + 1;                  /* include 0..n            */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);                                /* allocation failure      */
    cache = tmp;
    /* initialise new entries to zero                 */
    for (size_t i = max_computed + 1; i < new_size; ++i)
        cache[i] = 0;
}

/* Computes a(n) for any n >= 1, memoising results.                    */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined but safe       */
    if (cache == NULL) max_computed = 0;       /* first call               */
    if (max_computed >= n) return cache[n];    /* already computed          */

    ensure_cache(n);                           /* make room for up to n     */

    for (idx_t k = max_computed + 1; k <= n; ++k)
    {
        if (k <= 6) {                          /* initial block             */
            cache[k] = 1;
            continue;
        }

        idx_t m = k - 6;                       /* argument of convolution   */
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t e = m / d;
                if (d == e)
                    sum += cache[d] * cache[e];          /* i=j case           */
                else
                    sum += cache[d] * cache[e] + cache[e] * cache[d];
                    /* two ordered pairs (d,e) and (e,d)     */
            }
        }
        cache[k] = sum;
    }
    max_computed = n;
    return cache[n];
}
