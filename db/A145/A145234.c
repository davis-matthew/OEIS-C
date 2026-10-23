/*
 * A145234  a(n) = Fibonacci(7^n).
 * https://oeis.org/A145234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(7^n) Recurrence: a(0) = 1 a(n+1) = 125·a(n)^7 –
 *            175·a(n)^5 + 70·a(n)^3 – 7·a(n) (n ≥ 0)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The build provides types seq_t and idx_t.  seq_t is an unsigned
   integer type (8–128 bits or arbitrary‑precision); idx_t is an
   unsigned 64‑bit index. */

static seq_t *cache = NULL;   /* cache[0..last_computed] holds a(k) values */
static idx_t  cache_len   = 0;   /* allocated size of cache */
static idx_t  last_computed = 0;

/* Ensure the cache can hold index n.  Resize with realloc if needed. */
static inline void ensure_cache(idx_t n)
{
    if (n < cache_len) return;
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache = tmp;
    cache_len = new_len;
}

/* Return a(n), computing new terms on demand with the recurrence.  */
seq_t a(idx_t n)
{
    if (cache == NULL) {            /* initialise for first call */
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache_len = 1;
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        last_computed = 0;
    }

    if (n >= cache_len) {           /* need more cache space */
        ensure_cache(n);
    }

    if (n > last_computed) {        /* compute terms up to n */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t v   = cache[i-1];          /* a(i-1) */
            seq_t v2  = v * v;               /* v^2 */
            seq_t v3  = v2 * v;              /* v^3 */
            seq_t v5  = v2 * v3;             /* v^5 */
            seq_t v7  = v2 * v5;             /* v^7 */
            seq_t next = (seq_t)125 * v7
                         - (seq_t)175 * v5
                         + (seq_t)70  * v3
                         - (seq_t)7   * v;
            cache[i] = next;
        }
        last_computed = n;
    }

    return cache[n];
}
