/*
 * A181526  Prime numbers such that the difference between any pair of digits
 *          is 1, 2, 3, 5 or 7.
 * https://oeis.org/A181526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digit_len, get_digits, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the nth prime such that the absolute difference between any two adjacent digits of the prime is one
 * of {1, 2, 3, 5, 7}.  a(1) = 2, a(2) = 3, a(3) = 5, a(4) = 7, …
 */

#include <stdbool.h>
#include <stdint.h>          /* for uint64_t, size_t */
#include "../utils/is_prime.h"
#include "../utils/digit_len.h"
#include "../utils/get_digits.h"

static seq_t *cache = NULL;          /* array of found terms */
static size_t cache_cap = 0;
static size_t cache_len = 0;

static uint64_t cur_candidate = 0;   /* last tested candidate */

/* Ensure that the cache has room for at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_arr[i] = cache[i];
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* Check whether a prime candidate satisfies the adjacency‑difference property. */
static bool good_prime(uint64_t v)
{
    /* Extract decimal digits.  The utility stores most significant digit first. */
    int dlen = digit_len((seq_t)v);
    if (dlen <= 1)
        return true;                 /* 2,3,5,7 are single–digit primes */
    size_t len = (size_t)dlen;
    uint8_t digits[20];              /* enough for all 64‑bit values */
    size_t stored = len;             /* variable to satisfy get_digits prototype */
    get_digits((seq_t)v, digits, &stored);

    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            uint8_t diff = digits[i] > digits[j] ? digits[i] - digits[j] : digits[j] - digits[i];
            if (diff != 1 && diff != 2 && diff != 3 && diff != 5 && diff != 7)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Simple cache: generate terms up to the requested index on demand. */
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0, return 0 for safety */

    if (cache_len < (size_t)n) {
        /* Generate next terms until we have at least n of them. */
        while (cache_len < (size_t)n) {
            ++cur_candidate;
            if (cur_candidate < 2)
                continue;
            if (!is_prime(cur_candidate))
                continue;
            if (!good_prime(cur_candidate))
                continue;

            /* Store the prime in the cache. */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)cur_candidate;
        }
    }
    return cache[n - 1];
}
