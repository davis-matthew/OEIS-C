/*
 * A186815  Numbers n such that n^2-10 is a prime.
 * https://oeis.org/A186815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 19, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the nth integer > 0 for which n^2 – 10 is prime.
    The sequence is indexed from 1 (a(1) = 9).  The routine
    generates terms as needed and remembers them in a cache
    so subsequent calls are fast.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static size_t cache_len = 0;   /* number of terms stored */
static idx_t next_candidate = 9;   /* next integer to test */

static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* extend cache until it contains at least `up_to` terms */
static void grow_cache(idx_t up_to)
{
    ensure_capacity(up_to);
    while (cache_len < (size_t)up_to) {
        /* find next integer whose square minus 10 is prime */
        for (;;) {
            seq_t sq = (seq_t)next_candidate * (seq_t)next_candidate - (seq_t)10;
            if (is_prime_big(sq))
                break;
            next_candidate++;
        }
        cache[cache_len++] = (seq_t)next_candidate;
        next_candidate++;           /* next test starts after found value */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 0, but return 0 */
    if (cache_len < (size_t)n)
        grow_cache(n);
    return cache[n - 1];
}
