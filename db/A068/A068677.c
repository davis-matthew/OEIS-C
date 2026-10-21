/*
 * A068677  Numbers that yield primes when a 7 is prefixed or suffixed to
 *          them.
 * https://oeis.org/A068677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1246, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A068677: numbers n such that both 7n and n7 are prime numbers. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/concat_parts.h"
#include "../utils/isPrimeGeneric.h"

/* static storage for memoized terms */
static seq_t *cache = NULL;
static size_t  cache_len  = 0;
static size_t  cache_cap  = 0;
static idx_t   next_candidate = 1;  /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) {  /* behavior below the offset does not matter */
        return 0;
    }

    /* Return cached value if already computed */
    if (cache_len >= (size_t)n) {
        return cache[n - 1];
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        idx_t c = next_candidate++;

        /* quick sieving by small divisibility */
        if ((c & 1) == 0)                continue; /* even */
        if (c % 3 == 2)                  continue; /* would make both concatenations divisible by 3 */
        if (c % 5 == 0)                  continue; /* would make the prefixed concatenation divisible by 5 */

        /* construct the prefixes/suffixes */
        seq_t left  = concat_parts(7, c);   /* 7 followed by c */
        seq_t right = concat_parts(c, 7);   /* c followed by 7 */

        /* test primality */
        if (isPrimeGeneric(left) && isPrimeGeneric(right)) {
            /* ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));
            }
            cache[cache_len++] = c;
        }
    }

    return cache[n - 1];
}
