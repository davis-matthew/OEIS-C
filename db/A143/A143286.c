/*
 * A143286  Number of binary words of length n containing at least one subword
 *          10^{6}1 and no subwords 10^{i}1 with i<6.
 * https://oeis.org/A143286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-7) - a(n-9) - a(n-15); * Initial
 *            values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, *
 *            a(6)=0, a(7)=0, a(8)=1, a(9)=2, a(10)=3, a(11)=4, a(12)=5, *
 *            a(13)=6, a(14)=7, a(15)=9
 * Range:     u8 n <= 26, u16 n <= 48, u32 n <= 95, u64 n <= 192,
 *            u128 n <= 387, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache of computed terms. */
static seq_t *cache = NULL;              /* array of a(0)..a(current_max)   */
static size_t cache_len = 0;             /* number of elements in cache */

/* Base values for n = 0..15. */
static const seq_t BASE[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 2, 3, 4, 5, 6, 7, 9
};

/* Initialise cache with the base values. */
static void init_cache(void)
{
    if (cache_len != 0) return;         /* already initialised */
    cache_len = 16;
    cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
    for (idx_t i = 0; i < 16; ++i) cache[i] = BASE[i];
}

/* Extend the cache to contain values up to n. */
static void extend_cache(idx_t n)
{
    if ((size_t)n < cache_len) return;  /* already computed */
    size_t new_len = (size_t)n + 1;
    cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    /* Compute new values using the recurrence. */
    for (size_t i = cache_len; i < new_len; ++i) {
        seq_t val = cache[i-1] + cache[i-1]         /* 2*a(i-1) */
                   - cache[i-2]
                   + cache[i-7]
                   - cache[i-9]
                   - cache[i-15];
        cache[i] = val;
    }
    cache_len = new_len;
}

/* Public function. */
seq_t a(idx_t n)
{
    if (n < 16) return BASE[n];
    init_cache();
    extend_cache(n);
    return cache[n];
}
