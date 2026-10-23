/*
 * A155913  Primes p such that (sum of digits of p) - (last digit of p) is
 *          prime.
 * https://oeis.org/A155913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) returns the nth prime p such that (sum of digits of p) −
 *            (last digit of p) is prime
 * Range:     u8 n <= 18, u16 n <= 2245, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts 23, 29, 31, 37, 53, … (offset 1).  */
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/digit_sum.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

/* Static cache of result terms.  */
static seq_t *seq_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* The next integer to test when searching for the sequence. */
static idx_t candidate = 2;

/* Append a value to the cache, growing it if necessary. */
static void cache_append(seq_t value)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* memory allocation failed */
        seq_cache = tmp;
        cache_cap = new_cap;
    }
    seq_cache[cache_size++] = value;
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1; n==0 is outside of definition */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Find the next prime satisfying the property. */
        for (;;) {
            if (is_prime(candidate)) {
                seq_t val   = candidate;            /* convert to seq_t for digit_sum */
                seq_t sum   = digit_sum(val);
                seq_t last  = candidate % 10;
                seq_t diff  = sum - last;           /* sum ≥ last, diff is non‑negative */

                if (is_prime_big(diff)) {           /* diff must be a prime */
                    cache_append(candidate);
                    break;
                }
            }
            ++candidate;   /* continue with next integer */
        }
        ++candidate;   /* start searching from the next integer after the found prime */
    }

    return seq_cache[(size_t)n - 1];
}
