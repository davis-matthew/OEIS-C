/*
 * A082181  a(0) = 1, for n>=1, a(n) = Sum_{k=0..n} 9^k*N(n,k), where N(n,k) =
 *          (1/n)*C(n,k)*C(n,k+1) are the Narayana numbers (A001263).
 * https://oeis.org/A082181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Helpers:   mul_small_wider
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (10*(2*n-1)*a(n-1) - 64*(n-2)*a(n-2)) / (n+1) ;
 * a(0) = a(1) = 1
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small_wider.h"

/* compute a(n) */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ensure cache is large enough */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        /* initialise newly allocated entries to zero */
        for (idx_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_size = new_size;
    }

    /* base cases */
    if (cache[0] == 0) cache[0] = 1;
    if (cache_size > 1 && cache[1] == 0) cache[1] = 1;
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* recurrence – two separate variants for 64‑bit and larger seq_t   */
    if (sizeof(seq_t) <= 8) {          /* seq_t is 64 bit or smaller      */
        for (idx_t i = 2; i <= n; ++i) {
            if (cache[i] != 0) continue;   /* already computed */

            /* use 128‑bit for all intermediates to avoid overflow   */
            unsigned __int128 f1 = (unsigned __int128)10 *
                                   (((unsigned __int128)2 * i) - 1);
            unsigned __int128 f2 = (unsigned __int128)64 *
                                   ((unsigned __int128)i - 2);

            unsigned __int128 num = f1 * (unsigned __int128)cache[i-1]
                                   - f2 * (unsigned __int128)cache[i-2];

            cache[i] = (seq_t)(num / (unsigned __int128)(i + 1));
        }
    } else {                            /* seq_t is an arbitrary‑precision type */
        for (idx_t i = 2; i <= n; ++i) {
            if (cache[i] != 0) continue;

            seq_t prod1 = cache[i-1] *
                          (seq_t)(10ULL * (2ULL * i - 1ULL));
            seq_t prod2 = cache[i-2] *
                          (seq_t)(64ULL * (i - 2ULL));

            seq_t num   = prod1 - prod2;
            cache[i]    = num / (seq_t)(i + 1);
        }
    }

    return cache[n];
}
