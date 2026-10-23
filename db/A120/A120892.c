/*
 * A120892  a(n) = 3*a(n-1) + 3*a(n-2) - a(n-3); a(0) = 1, a(1) = 0, a(2) = 3.
 *          a(n) = 4*{a(n-1)+(-1)^n}-a(n-2); a(0) = 1, a(1) = 0.
 * https://oeis.org/A120892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - a(n-3); a(0)=1, a(1)=0, a(2)=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoises the sequence in a static array so that any
   call order is supported efficiently. */

#include <stdlib.h>

/* static cache for computed values */
static seq_t* cache = NULL;
static idx_t cache_size = 0;

/* initialise cache with base cases */
static void init_cache(void)
{
    if (cache != NULL) return;

    cache = (seq_t*)malloc(3 * sizeof(seq_t));
    if (cache == NULL) exit(1);           /* cannot allocate */
    cache[0] = 1;
    cache[1] = 0;
    cache[2] = 3;
    cache_size = 3;
}

/* grow cache up to index n (inclusive) */
static void grow_cache(idx_t n)
{
    if (n < cache_size) return;

    seq_t* new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL) exit(1);       /* cannot reallocate */

    /* compute new entries using recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        new_cache[i] = 3 * new_cache[i - 1]
                       + 3 * new_cache[i - 2]
                       - new_cache[i - 3];
    }

    cache = new_cache;
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    init_cache();
    if (n < cache_size) {
        return cache[n];
    }
    grow_cache(n);
    return cache[n];
}
