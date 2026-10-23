/*
 * A033062  Numbers whose base-11 representation Sum_{i=0..m} d(i)*11^i has
 *          odd d(m-i) for all odd i.
 * https://oeis.org/A033062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑11 representation
 *            satisfies: for all odd i, the digit at position m‑i (where m is
 *            the highest index) is odd. The routine computes terms by a
 *            simple brute‑force search, caching results
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for the computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;     /* number of terms stored */
static uint64_t next_val = 1;     /* next integer to test */

/* Check whether a 64‑bit integer satisfies the base‑11 digit rule. */
static bool valid(uint64_t x)
{
    if (x == 0) return false;        /* numbers start from 1 */

    int digits[32];
    int len = 0;
    uint64_t t = x;
    while (t > 0) {
        digits[len++] = (int)(t % 11);
        t /= 11;
    }

    int m = len - 1;                 /* index of the most significant digit */
    for (int j = 0; j < len; ++j) {
        if (((m - j) & 1) == 1) {    /* position requires an odd digit */
            if ((digits[j] & 1) == 0) return false;
        }
    }
    return true;
}

/* Grow the cache array to at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);    /* abort on allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset: a(1) is the first term */

    size_t target = (size_t)n;
    while (cache_size < target) {
        /* Find the next valid integer. */
        while (!valid(next_val)) ++next_val;

        grow_cache(cache_size + 1);
        cache[cache_size++] = (seq_t)next_val;
        ++next_val;
    }
    return cache[target - 1];
}
