/*
 * A116526  a(0)=1, a(1)=1, a(n) = 9*a(n/2) for even n >= 2, and a(n) =
 *          8*a((n-1)/2) + a((n+1)/2) for odd n >= 3.
 * https://oeis.org/A116526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, for n >= 2: if n even -> a(n) = 9 * a(n/2)
 *            if n odd -> a(n) = 8 * a((n-1)/2) + a((n+1)/2)
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 1471, u64 n <= 9333,
 *            u128 n <= 9333, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses memoisation so that repeated calls with the same
   index are fast and the recursion depth is at most O(log n). */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* memoised values */
static unsigned char *present = NULL;  /* flag whether cache[i] is valid */
static idx_t cache_size = 0;   /* current capacity of cache arrays */

static void ensure_capacity(idx_t n)
{
    if (n < cache_size)
        return;

    idx_t new_size = (cache_size == 0) ? (n + 1) : (cache_size << 1);
    if (new_size <= n)
        new_size = n + 1;

    seq_t *tmp_cache   = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    unsigned char *tmp_present = (unsigned char *)realloc(present,
                                                             new_size *
                                                             sizeof(unsigned char));
    if (!tmp_cache || !tmp_present)
        abort();   /* allocation failed */

    cache   = tmp_cache;
    present = tmp_present;

    for (idx_t i = cache_size; i < new_size; ++i)
        present[i] = 0;

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);

    if (present[n])
        return cache[n];

    seq_t val;
    if (n == 0) {
        val = 0;
    } else if (n == 1) {
        val = 1;
    } else if ((n & 1) == 0) {
        val = (seq_t)9 * a(n / 2);
    } else {
        val = (seq_t)8 * a((n - 1) / 2) + a((n + 1) / 2);
    }

    cache[n] = val;
    present[n] = 1;

    return val;
}
