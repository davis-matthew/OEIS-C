/*
 * A154711  Squares such that square-+5 are primes.
 * https://oeis.org/A154711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154711: squares n^2 such that n^2-5 and n^2+5 are prime. */
/* Calculation uses a simple incremental search with memoisation. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* declares bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* Static state for memoisation across calls. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t checked = 0;          /* last integer checked for the property */

    if (n == 0)        /* the sequence is 1‑based; 0 is undefined */
        return 0;

    /* Extend cache until it holds at least n terms. */
    while (cache_size < n) {
        ++checked;                     /* next candidate integer */

        /* Compute checked² as a seq_t value. */
        seq_t sq = (seq_t)checked;
        sq *= sq;

        /* Skip trivial cases where sq-5 would not be prime. */
        if (sq <= 5) continue;

        /* Test primality of sq-5 and sq+5. */
        if (is_prime_big(sq - 5) && is_prime_big(sq + 5)) {
            /* Grow the cache if necessary. */
            if (cache_size == cache_cap) {
                size_t newcap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_size++] = sq;
        }
    }

    return cache[n - 1];
}
