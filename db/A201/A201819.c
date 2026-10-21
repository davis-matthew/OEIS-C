/*
 * A201819  Numbers n such that 90*n + 31 is prime.
 * https://oeis.org/A201819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 274, u32 n <= 62, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute terms by searching for n such that 90*n + 31 is prime. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found n's */
    static size_t cap     = 0;     /* current capacity */
    static idx_t  len      = 0;    /* number of stored terms */
    static idx_t  next_m   = 1;    /* next candidate n to test */

    if (!cache) {
        /* initialise cache with the first term a(1)=0 */
        cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = (seq_t)0;
        len = 1;
    }

    /* keep generating until we have at least n terms */
    while (len < n) {
        seq_t val = (seq_t)90 * (seq_t)next_m + (seq_t)31;
        if (is_prime_big(val)) {
            if (len == cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = (seq_t)next_m;
        }
        ++next_m;
    }

    return cache[n - 1];
}
