/*
 * A145122  Numbers X such that (X+19)^3-X^3 is a square.
 * https://oeis.org/A145122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=38, a(2)=14383,
   a(n) = 302*a(n-1) - a(n-2) + 2850        for n ≥ 3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Local static cache of computed terms; indices 0..cache_len-1 correspond to a(1)..a(cache_len) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of cached terms */

    /* n must be ≥ 1 by contract; if 0 passed, just return 0 to avoid undefined behavior */
    if (n == 0)
        return 0;

    /* If the requested term is not yet cached, grow and compute */
    if (n > cache_len) {
        idx_t new_len = n;

        /* Reallocate cache to hold new_len terms; cast the void* result to seq_t* for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            exit(1);                 /* allocation failed */
        cache = tmp;

        /* Ensure the first two terms are initialized (a(1)=38, a(2)=14383) */
        if (cache_len == 0 && new_len >= 1)
            cache[0] = (seq_t)38;    /* a(1) */
        if (cache_len <= 1 && new_len >= 2)
            cache[1] = (seq_t)14383; /* a(2) */

        /* Compute remaining terms using the recurrence */
        for (idx_t k = (cache_len < 2 ? 2 : cache_len); k < new_len; ++k) {
            seq_t term = 302 * cache[k - 1];
            term -= cache[k - 2];
            term += (seq_t)2850;
            cache[k] = term;
        }

        cache_len = new_len;
    }

    return cache[n - 1];
}
