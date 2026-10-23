/*
 * A164592  a(n) = 10*a(n-1) - 17*a(n-2) for n > 1; a(0) = 1, a(1) = 8.
 * https://oeis.org/A164592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 17*a(n-2); a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Maximum n the routine comfortably supports (≥ 10000) */
#define MAX_TERM 20000

/* Static cache for computed values */
static seq_t cache[MAX_TERM + 1];
static idx_t cache_size = 0;

/* Compute the nth term of OEIS sequence A164592 */
seq_t a(idx_t n)
{
    /* Initialize the base cases once */
    if (cache_size == 0) {
        cache[0] = 1;  /* a(0) */
        cache[1] = 8;  /* a(1) */
        cache_size = 2;
    }

    /* If requested value is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = 10 * cache[i - 1] - 17 * cache[i - 2];
    }
    cache_size = n + 1;
    return cache[n];
}
