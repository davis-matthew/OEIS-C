/*
 * A108662  Numbers whose sum of squares of digits is a prime.
 * https://oeis.org/A108662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sumsq_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generating A108662: numbers whose sum of squares of decimal digits is prime */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sumsq_digits.h"

/* Small primality test for the small sums (maximum ≈ 3200). */
static inline bool is_prime_small(unsigned n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (unsigned i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

/* Memoised generator of the sequence. */
static seq_t *cache = NULL;          /* cached terms */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cache_size = 0;        /* number of terms stored */
static idx_t next_candidate = 1;     /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined but harmless */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        seq_t val = (seq_t)next_candidate;
        next_candidate++;

        if (is_prime_small(sumsq_digits(val))) {
            if (cache_size >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = val;
        }
    }

    return cache[(size_t)n - 1];
}
