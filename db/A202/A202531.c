/*
 * A202531  Number of (n+2) X 9 binary arrays avoiding patterns 000 and 101 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) - 2*a(n-4) - 2*a(n-5) + a(n-7) for n
 *            > 8 initial values: a(1)=8994, a(2)=22525, a(3)=47098,
 *            a(4)=97936, a(5)=214388, a(6)=462445, a(7)=986000, a(8)=2119913
 * Range:     u8 none, u16 n <= 3, u32 n <= 17, u64 n <= 46, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static int initialized = 0;

static void init_cache(idx_t n)
{
    if (initialized) return;
    /* Allocate space for at least the first 8 values */
    idx_t needed = (n > 8 ? n+1 : 9);
    cache = (seq_t*)realloc(cache, sizeof(seq_t)*needed);
    cache_capacity = needed;
    /* initialize all to zero */
    for (idx_t i = 0; i < needed; ++i) cache[i] = 0;
    /* base values, indices start at 1 */
    cache[1] = 8994;
    cache[2] = 22525;
    cache[3] = 47098;
    cache[4] = 97936;
    cache[5] = 214388;
    cache[6] = 462445;
    cache[7] = 986000;
    cache[8] = 2119913;
    initialized = 1;
}

static void ensure_cache(idx_t n)
{
    if (n < cache_capacity) return;
    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t)*new_cap);
    cache = new_cache;
    /* zero the new area */
    for (idx_t i = cache_capacity; i < new_cap; ++i) cache[i] = 0;
    cache_capacity = new_cap;
}

/* External function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1, return 0 as safe fallback */
    init_cache(n);
    ensure_cache(n);
    /* Compute all terms up to n that are not yet computed */
    for (idx_t i = 9; i <= n; ++i) {
        if (cache[i] != 0) continue;   /* already computed (in case of re‑calls larger n) */
        cache[i] = 2*cache[i-1]
                 + 2*cache[i-3]
                 - 2*cache[i-4]
                 - 2*cache[i-5]
                 + cache[i-7];
    }
    return cache[n];
}
