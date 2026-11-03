/*
 * A101625  A bisection of the Stern-Jacobsthal numbers.
 * https://oeis.org/A101625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) XOR (a(n-2)*4); a(0) = 0, a(1) = 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 892 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of previously computed terms   */
    static idx_t cap   = 0;        /* allocated number of elements         */
    static idx_t len   = 0;        /* number of elements filled in cache   */

    /* Ensure the cache has space for indices 0 .. n */
    if (cache == NULL) {
        /* first allocation: at least two elements for a(0) and a(1) */
        cap = 2;
        if ((cache = mem_new(seq_t, cap)) == NULL)
            abort();                  /* fatal allocation failure */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        len = 2;
    } else if (n >= cap) {
        /* enlarge cache to at least (n + 1) elements, doubling each time */
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (tmp == NULL) abort();      /* fatal allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* If we already have the nth term, just return it */
    if (n < len)
        return cache[n];

    /* Compute terms up to n using the recurrence */
    for (idx_t i = len; i <= n; ++i) {
        seq_t term = cache[i - 1] ^ (cache[i - 2] << 2);
        cache[i] = term;
    }
    len = n + 1;          /* we now have values up to n     */
    return cache[n];      /* return the requested term       */
}
