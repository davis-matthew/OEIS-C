/*
 * A050712  Inserting a digit '2' between adjacent digits of n makes a prime.
 * https://oeis.org/A050712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer n (with at least two decimal digits) such
 *            that inserting the digit '2' between every pair of adjacent
 *            digits of n produces a prime number. The sequence starts with
 *            17, 23, 27, 29, …
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 123, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/is_prime_big.h"

static seq_t insert_two (seq_t x)
{
    /* Build the number obtained by writing each decimal digit of x
       and inserting the digit 2 between successive digits. */
    uint8_t buf[128];
    size_t len = 0;
    get_digits (x, buf, &len);              /* least significant first */

    seq_t res = 0;
    if (len == 0) return 0;

    for (int i = (int)len - 1; i >= 0; --i) {
        res = res * 10 + buf[i];
        if (i > 0)                     /* insert '2' between digits */
            res = res * 10 + 2;
    }
    return res;
}

/* Static cache to memoise the sequence values across calls. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static seq_t  next_candidate = 10;   /* start checking from 10 */

seq_t a (idx_t n)
{
    if (n == 0) return 0;                           /* safety for offset */

    /* Grow the cache until we have at least n elements. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate++;
        seq_t inserted = insert_two (cand);
        if (is_prime_big (inserted)) {
            /* Ensure sufficient capacity. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc (cache, new_cap * sizeof (seq_t));
                if (!tmp) abort();  /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }
    return cache[n-1];
}
