/*
 * A085520  Numbers whose digits alternate between prime and nonprime.
 * https://oeis.org/A085520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th smallest integer whose decimal digits alternate
 *            * between prime (2,3,5,7) and non‑prime (0,1,4,6,8,9). The
 *            sequence * starts with a(1)=12. This implementation generates
 *            terms by brute‑force * enumeration up to the requested index
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alternates, is_prime_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/alternates.h"
#include "../utils/is_prime_digit.h"

/* local helper: true if d is a prime digit */

/* returns the n-th term of A085520; n is 1‑based, n>=1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* offset is 1; below we return 0 */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t last = (seq_t)11;    /* largest number tested so far */

    /* initialise storage once */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* return cached value if already known */
    if (n <= cache_len)
        return cache[n - 1];

    /* enumerate new numbers until we reach the desired index */
    seq_t num = last + (seq_t)1;
    while (cache_len < n) {
        if (num >= (seq_t)10 && alternates(num)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) /* malloc failure – abort */
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = num;
        }
        ++num;
    }
    last = num - (seq_t)1;
    return cache[n - 1];
}
