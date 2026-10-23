/*
 * A155030  Primes p=A000040(n) with prime index n such that the concatenation
 *          n//p is a composite number.
 * https://oeis.org/A155030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 14, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   concat_parts, get_prime, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = prime(n) when n is a prime index and concat(n,prime(n)) is composite */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    /* cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 1;  /* last index examined */

    /* grow cache until it contains at least n terms */
    while (cache_len < n) {
        /* look at the next index */
        ++last_checked;
        /* only consider prime indices */
        if (!is_prime(last_checked))
            continue;

        /* prime at this index */
        uint32_t pk = get_prime(last_checked);
        seq_t prime = (seq_t)pk;

        /* concatenation of index and prime */
        seq_t concat = concat_parts(last_checked, (idx_t)prime);

        /* if the concatenation is composite, store the prime */
        if (!is_prime_big(concat)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();          /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = prime;
        }
    }

    return cache[n - 1];
}
