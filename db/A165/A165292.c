/*
 * A165292  Primes obtained from other primes by pre-concatenating with 3.
 * https://oeis.org/A165292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   * a(n) is the n‑th prime of the form 3·10^k + p where p is
 *            itself a prime. * The function generates values by scanning
 *            successive primes p and * testing the concatenated number for
 *            primality
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   digit_count, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Provided helper functions */
#include "../utils/is_prime_big.h"
#include "../utils/digit_count.h"

/* Cache of found terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Next prime candidate to test */
static uint64_t next_candidate = 2;

/* Append a value to the cache, growing it if needed */
static void cache_push(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Return 10^(number of decimal digits of n) as a seq_t */
static seq_t pow10_of_digits(uint64_t n)
{
    size_t d = digit_count(n);
    seq_t p = 1;
    for (size_t i = 0; i < d; ++i)
        p *= (seq_t)10;
    return p;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                         /* undefined for n < 1 */
        return 0;

    /* Generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        /* Test whether the current candidate is prime */
        if (is_prime_big((seq_t)next_candidate)) {
            seq_t p10 = pow10_of_digits(next_candidate);
            seq_t q = (seq_t)next_candidate + ((seq_t)3) * p10;
            if (is_prime_big(q))
                cache_push(q);
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
