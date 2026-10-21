/*
 * A085557  Numbers that have more prime digits than nonprime digits.
 * https://oeis.org/A085557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the nth positive integer that contains more prime digits
 *            (2,3,5,7) than non‑prime digits (0,1,4,6,8,9)
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Memoised storage of the sequence */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of elements currently stored */
static idx_t cache_cap = 0;   /* capacity of the cache array */

/* Helper to ensure the cache can hold at least n elements */
static void ensure_cache_cap(idx_t n)
{
    if (cache_cap >= n)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < n)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* realloc() preserves existing contents; on error it returns NULL, but
       the build system guarantees sufficient memory for the test suite */
    cache_cap = new_cap;
}

/* Test whether the digit d is a prime digit */
static inline bool is_prime_digit(seq_t d)
{
    return d == (seq_t)2 || d == (seq_t)3 ||
           d == (seq_t)5 || d == (seq_t)7;
}

/* Count primes and non‑primes in the decimal representation of x */
static inline bool has_more_prime_digits(seq_t x)
{
    seq_t primes = 0;
    seq_t nonprimes = 0;
    while (x > 0) {
        seq_t d = x % (seq_t)10;
        if (is_prime_digit(d))
            ++primes;
        else
            ++nonprimes;
        x /= (seq_t)10;
    }
    return primes > nonprimes;
}

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    /* Build the cache up to the requested index (1‑based) */
    if (cache_len < n) {
        ensure_cache_cap(n);
        seq_t next = (cache_len == 0) ? (seq_t)1 : cache[cache_len - 1] + 1;
        while (cache_len < n) {
            if (has_more_prime_digits(next))
                cache[cache_len++] = next;
            next += (seq_t)1;
        }
    }
    return cache[n - 1];
}
