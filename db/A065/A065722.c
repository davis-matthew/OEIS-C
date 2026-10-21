/*
 * A065722  Primes that when written in base 4, then reinterpreted in base 10,
 *          again give primes.
 * https://oeis.org/A065722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime p such that interpreting the * base‑4
 *            representation of p as a decimal integer yields another * prime.
 *            The sequence is defined for n ≥ 1
 * Range:     u8 n <= 13, u16 n <= 130, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   conv_base4_to_decimal, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"     /* bool is_prime(idx_t n); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */
#include "../utils/conv_base4_to_decimal.h"

static seq_t *seq_cache = NULL;   /* cached sequence values */
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static idx_t primes_checked = 1;  /* last integer examined */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(idx_t needed)
{
    if (cache_cap >= (size_t)needed) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* sequence offset is 1 */
    while (cache_len < n) {
        /* Find next prime number. */
        do { ++primes_checked; } while (!is_prime(primes_checked));

        /* Convert to decimal via base‑4 digits. */
        seq_t conv = conv_base4_to_decimal(primes_checked);

        /* Check primality of the converted value. */
        if (is_prime_big(conv)) {
            ensure_cache_capacity(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)primes_checked;
        }
    }
    return seq_cache[n-1];
}
