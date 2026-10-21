/*
 * A063163  Composite numbers which in base 7 contain their largest proper
 *          factor as a substring.
 * https://oeis.org/A063163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A063163 – composite numbers whose base‑7 representation contains
   their largest proper factor as a substring (search). */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Convert a 64‑bit integer to a null‑terminated base‑7 string.
   Assumes buf has at least 32 bytes. */
static size_t to_base7(uint64_t v, char *buf)
{
    size_t i = 0;
    if (v == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
        return i;
    }
    while (v > 0) {
        buf[i++] = '0' + (v % 7);
        v /= 7;
    }
    /* Reverse the string in place. */
    for (size_t l = 0, r = i - 1; l < r; ++l, --r) {
        char tmp = buf[l];
        buf[l] = buf[r];
        buf[r] = tmp;
    }
    buf[i] = '\0';
    return i;
}

/* Test whether the base‑7 representation of x contains the base‑7
   representation of y as a substring. */
static bool contains_substring(uint64_t x, uint64_t y)
{
    char buf_x[32], buf_y[32];
    to_base7(x, buf_x);
    to_base7(y, buf_y);
    return strstr(buf_x, buf_y) != NULL;
}

/* Return the smallest prime factor of n; if n is prime it returns n. */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n <= 3) return n;
    if ((n & 1) == 0) return 2;
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) return p;
    }
    return n; /* prime */
}

/* Cached sequence values. */
static seq_t   *cache     = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;

/* Grow the cache to at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* If we have already computed enough terms, return the cached value. */
    if ((size_t)n <= cache_len) return cache[(size_t)n - 1];

    /* Static state for incremental search. */
    static uint64_t candidate = 2;

    /* Continue searching until we have computed n terms. */
    while (cache_len < (size_t)n) {
        /* Find next composite number. */
        do {
            ++candidate;
        } while (smallest_prime_factor(candidate) == candidate);

        uint64_t spf    = smallest_prime_factor(candidate);
        uint64_t largest = candidate / spf;          /* largest proper factor */

        if (contains_substring(candidate, largest)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)candidate;
        }
    }

    return cache[(size_t)n - 1];
}
