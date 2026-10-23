/*
 * A176616  Primes of the form x^2 + 7*y^2, where x and y=x+1 are consecutive
 *          natural numbers.
 * https://oeis.org/A176616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*(k-1)^2 + 14*(k-1) + 7, for k >= 1, and only when the *
 *            value is prime. a(1) = 7, a(2) = 29,
 * Range:     u8 n <= 4, u16 n <= 36, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Ensure we can store at least 'needed' elements in *ptr. */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
    if (new_cap < needed) new_cap = needed;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

/* Cache of found primes of the required form. */
static seq_t *prime_cache = NULL;
static size_t cache_cap = 0;
static size_t prime_count = 0;
static idx_t next_x = 0;  /* Next x to test (x = n-1). */

seq_t a(idx_t n)
{
    /* n >= 1 by contract. */
    while (prime_count < n) {
        /* Compute 8*x^2 + 14*x + 7, with x = next_x. */
        seq_t xsq = (seq_t)next_x * (seq_t)next_x;
        seq_t val = (seq_t)8 * xsq + (seq_t)14 * (seq_t)next_x + (seq_t)7;

        if (is_prime_big(val)) {
            if (prime_count == cache_cap)
                ensure_capacity(cache_cap > 0 ? cache_cap << 1 : 16,
                               &cache_cap, (void **)&prime_cache, sizeof(seq_t));
            prime_cache[prime_count++] = val;
        }
        ++next_x;
    }
    return prime_cache[n - 1];
}
