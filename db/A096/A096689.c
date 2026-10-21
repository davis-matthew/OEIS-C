/*
 * A096689  Numbers n such that 2n^2 + 3n + 3 is prime.
 * https://oeis.org/A096689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer m such that 2*m*m + 3*m + 3 is prime
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 28, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache of values that satisfy the condition. */
static seq_t *cache      = NULL;   /* array of found n */
static size_t cache_size = 0;      /* number of elements in cache   */
static size_t cache_cap  = 0;      /* allocated capacity              */
static idx_t next_test    = 0;      /* next integer to test           */

/* a(n) returns the n-th term of A096689, with offset 1. */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset 1, undefined for 0 */
        return 0;

    /* Extend the cache until it contains at least n elements. */
    while (cache_size < n) {
        seq_t t   = (seq_t)next_test;                     /* current n to test */
        seq_t val = 2 * t * t + 3 * t + 3;                /* 2*n^2+3*n+3     */
        if (is_prime_big(val)) {                          /* primality test   */
            /* Grow the cache if necessary. */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                /* In the unlikely event realloc fails we abort. */
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = t;                      /* store valid n   */
        }
        ++next_test;                                      /* next integer    */
    }
    return cache[n - 1];                                  /* return cached n */
}
