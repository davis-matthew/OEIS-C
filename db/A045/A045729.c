/*
 * A045729  Fibonacci numbers having initial digit '5'.
 * https://oeis.org/A045729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find Fibonacci numbers whose first digit is 5. */
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached terms */
static size_t cache_len = 0;         /* number of cached terms */
static size_t cache_cap = 0;         /* capacity of cache */
static seq_t fib_prev = 0;           /* previous Fibonacci number */
static seq_t fib_cur  = 1;           /* current Fibonacci number */
static int initialized = 0;         /* flag indicating first call */

/* return the first decimal digit of x */
static seq_t first_digit(seq_t x)
{
    while (x >= (seq_t)10) {
        x /= (seq_t)10;
    }
    return x;
}

/* enlarge the cache if needed */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) return; /* abort on OOM */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = 1;
        cache_len = 0;
        cache_cap = 0;
        cache = NULL;
    }

    /* grow cache until we have at least n terms */
    while (cache_len < n) {
        if (first_digit(fib_cur) == (seq_t)5) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = fib_cur;
        }
        /* advance to next Fibonacci number */
        seq_t next = fib_prev + fib_cur;
        fib_prev = fib_cur;
        fib_cur  = next;
    }

    return cache[n - 1];
}
