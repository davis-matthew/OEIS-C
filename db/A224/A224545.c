/*
 * A224545  Number of (n+1) X 4 0..1 matrices with each 2 X 2 subblock
 *          idempotent.
 * https://oeis.org/A224545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5)  (n>6)
   Base values (offset=1): a(1)=28, a(2)=52, a(3)=78, a(4)=125, a(5)=193, a(6)=302 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based storage: cache[1] = a(1) */
static idx_t  cache_len = 0;     /* highest index currently cached */
static size_t mem_cap   = 0;      /* capacity of cache in elements */

/* Ensure cache can hold values up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_len) return;

    if (!cache || (size_t)(n + 1) > mem_cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 16) new_cap = 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        mem_cap = new_cap;
    }

    static const seq_t base[6] = {28, 52, 78, 125, 193, 302};
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = base[i - 1];
        } else {
            seq_t term = (seq_t)4 * cache[i - 1]
                       + cache[i - 3]
                       + (seq_t)2 * cache[i - 4]
                       - (seq_t)5 * cache[i - 2]
                       - cache[i - 5];
            cache[i] = term;
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */
    ensure_capacity(n);
    return cache[n];
}
