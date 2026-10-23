/*
 * A125180  a(n) = 2*a(n-1) + prime(n) - prime(n-1), a(1)=2, where prime(n)
 *          denotes the n-th prime.
 * https://oeis.org/A125180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + prime(n) - prime(n-1), a(1)=2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   alloc_seq, free_seq_array, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses an expanding static cache so that calls
   can be made in any order.  It depends on the utility functions
   alloc_seq, free_seq_array and get_prime, which are provided in
   the ../utils directory. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq_array.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] unused; cache[1..cache_len] valid */
    static idx_t cache_len = 0;   /* largest index computed so far */

    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined for n=0, safe return */

    if (n > cache_len) {
        /* Allocate space for indices 0..n (index 0 unused) */
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = alloc_seq(new_len);
        if (!new_cache)
            abort();

        /* Copy existing values into the new cache */
        if (cache) {
            for (idx_t i = 1; i <= cache_len; ++i)
                new_cache[i] = cache[i];
        }

        /* Compute new values from cache_len+1 up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == (idx_t)1) {
                new_cache[1] = (seq_t)2;
            } else {
                seq_t prev = new_cache[i - 1];
                uint32_t pi  = get_prime(i);
                uint32_t pim = get_prime(i - 1);
                seq_t delta = (seq_t)(pi - pim);
                new_cache[i] = (seq_t)(prev * (seq_t)2 + delta);
            }
        }

        /* Free old cache and commit new one */
        if (cache)
            free_seq_array(cache);
        cache     = new_cache;
        cache_len = n;
    }

    return cache[n];
}
