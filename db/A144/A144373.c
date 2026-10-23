/*
 * A144373  Shifts 9 places left under Dirichlet convolution.
 * https://oeis.org/A144373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(1..9)=1; for n>=10, a(n)=∑_{d|(n-9)} a(d)*a((n-9)/d)
 * Range:     u8 n <= 59, u16 n <= 128, u32 n <= 272, u64 n <= 560,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence derived from the generating function:
   G(x) = x + ... + x^9 + x^9*(1+∑_{i≥1}∑_{j≥1} a(i)a(j)x^{i*j})   */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of a(1..max_n), index 0 unused */
static size_t cache_max = 0;         /* largest n that has been computed */

/* Ensure cache has room for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_max >= n) return;
    size_t new_max = cache_max ? cache_max * 2 : 256;
    while (new_max < (size_t)n) new_max <<= 1;
    seq_t *new_seq = (seq_t *)realloc(cache, (new_max + 1) * sizeof(seq_t));
    /* initialize the new part to zero (all a(i) will be set before use) */
    for (size_t i = cache_max + 1; i <= new_max; ++i) new_seq[i] = 0;
    cache = new_seq;
    cache_max = new_max;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, but guard anyway */

    ensure_cache(n);

    /* compute any entries that are still zero up to n */
    for (idx_t k = 1; k <= n; ++k)
    {
        if (cache[k] != 0) continue;  /* already computed (for k <=9 set to 1) */

        if (k <= 9)
        {
            cache[k] = (seq_t)1;
        }
        else
        {
            idx_t m = k - 9;          /* m >= 1 */
            seq_t sum = 0;
            for (idx_t d = 1; d * d <= m; ++d)
            {
                if (m % d == 0)
                {
                    idx_t e = m / d;
                    seq_t prod = cache[d] * cache[e];
                    sum += prod;
                    if (d != e) sum += prod;   /* add a(e)*a(d) separately */
                }
            }
            cache[k] = sum;
        }
    }

    return cache[n];
}
