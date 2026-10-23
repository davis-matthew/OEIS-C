/*
 * A024945  a(n) = Sum_{i=1..floor((n+2)/4)} a(2i-1)*a(n-2i+1), with
 *          a(1)=a(2)=1 and a(3)=2.
 * https://oeis.org/A024945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..⌊(n+2)/4⌋} a(2i-1)*a(n-2i+1) with a(1)=a(2)=1,
 *            a(3)=2 (offset 1)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;     /* 1‑based cache of computed terms   */
static idx_t  max_computed = 0; /* largest n that has been computed  */
static size_t capacity    = 0;   /* allocated size of cache (>= n+1) */

/* Ensure the cache can hold indices up to n (inclusive). */
static void grow_cache_to(idx_t n)
{
    if (n + 1 <= capacity)
        return;
    size_t new_cap = (size_t)(n + 1) * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);                      /* allocation failure – should not happen */
    cache = new_cache;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* sequence starts at n = 1 */

    if (n <= max_computed)            /* already computed */
        return cache[n];

    /* compute all terms up to n */
    grow_cache_to(n);
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        seq_t val;
        if (m == 1 || m == 2)
            val = 1;
        else if (m == 3)
            val = 2;
        else {
            seq_t sum = 0;
            idx_t limit = (m + 2) / 4;           /* floor((m+2)/4) */
            for (idx_t i = 1; i <= limit; ++i) {
                idx_t k = 2 * i - 1;              /* odd index */
                idx_t j = m - 2 * i + 1;          /* complementary index */
                sum += cache[k] * cache[j];
            }
            val = sum;
        }
        cache[m] = val;
    }
    max_computed = n;
    return cache[n];
}
