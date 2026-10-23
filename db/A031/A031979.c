/*
 * A031979  Smallest digit of n concatenated with largest digit of n is prime.
 * https://oeis.org/A031979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th positive integer m such that (smallest digit of
 *            m concatenated with largest digit of m) is a prime
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_digits, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/is_prime_small.h"

static bool valid_value(seq_t x)
{
    uint8_t digits[64];
    size_t len = 0;
    get_digits(x, digits, &len);

    uint8_t min = 9, max = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = digits[i];
        if (d < min) min = d;
        if (d > max) max = d;
    }

    int concat = min * 10 + max;
    return is_prime_small((seq_t)concat);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n<1 */

    static seq_t *cache    = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Expand cache storage if needed */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1); /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t cand = (cache_len == 0) ? (seq_t)2 : cache[cache_len-1] + (seq_t)1;
        while (!valid_value(cand)) ++cand;
        cache[cache_len++] = cand;
    }

    return cache[n-1];
}
