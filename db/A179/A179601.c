/*
 * A179601  Eight white kings and one red king on a 3 X 3 chessboard. G.f.:
 *          (1+4*x)/(1 - 2*x - 10*x^2 - 4*x^3).
 * https://oeis.org/A179601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 10*a(n-2) + 4*a(n-3), a(0)=1, a(1)=6, a(2)=22
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache      = NULL;   /* pointer to array of values */
    static idx_t cache_size  = 0;      /* current capacity of the array */
    static idx_t next_to_compute = 0;  /* next index that must be filled */

    /* Determine the required array size.
       It must be at least n+1 and at least 3 to store the initial values. */
    idx_t required = (n + 1 > cache_size) ? n + 1 : cache_size;
    if (required < 3) required = 3;

    /* Reallocate if necessary (C++ requires a cast, C accepts implicit cast) */
    if (required > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) abort();          /* out–of–memory: aborting is acceptable */
        cache = tmp;
        cache_size = required;
    }

    /* Initialise first three terms on first use */
    if (next_to_compute == 0) {
        cache[0] = 1;
        cache[1] = 6;
        cache[2] = 22;
        next_to_compute = 3;
    }

    /* Compute any remaining terms up to n */
    while (next_to_compute <= n) {
        /* Recurrence: a(i) = 2*a(i-1) + 10*a(i-2) + 4*a(i-3) */
        cache[next_to_compute] = 2 * cache[next_to_compute - 1]
                               + 10 * cache[next_to_compute - 2]
                               + 4 * cache[next_to_compute - 3];
        ++next_to_compute;
    }

    return cache[n];
}
