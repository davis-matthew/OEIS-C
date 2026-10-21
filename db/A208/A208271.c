/*
 * A208271  Nonprime numbers containing a digit 1.
 * https://oeis.org/A208271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth nonprime number that contains the digit 1.
   The sequence is OEIS A208271.  It is computed by a simple sieve that
   keeps a cache of already-found terms so that any order of calls works. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/is_prime_big.h"

/* Check whether the decimal representation of x contains the digit 1. */
static bool contains_one(seq_t x)
{
    uint8_t buf[200];
    size_t len;
    get_digits(x, buf, &len);     /* buf stores digits 0‑9 */
    for (size_t i = 0; i < len; ++i) {
        if (buf[i] == 1)          /* digit 1 found */
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset 1: a(1) is the first term */
        return 0;

    /* Static cache: cache[0] == a(1), cache[1] == a(2), ... */
    static seq_t *cache       = NULL;   /* array of cached terms */
    static size_t cache_len   = 0;      /* number of terms cached so far */
    static size_t cache_cap   = 0;      /* current capacity of cache array */
    static seq_t next_candidate = 1;    /* next number to test */

    /* Grow cache capacity if necessary */
    if (n > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? n : cache_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure (unlikely) */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until the cache reaches the required size */
    while (cache_len < (size_t)n) {
        seq_t cur = next_candidate;
        /* a nonprime number containing the digit 1 */
        if (!is_prime_big(cur) && contains_one(cur))
            cache[cache_len++] = cur;
        next_candidate = cur + 1;
    }

    return cache[n - 1];
}
