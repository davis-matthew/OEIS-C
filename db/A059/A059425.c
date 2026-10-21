/*
 * A059425  Primes of form n^2 + 19n + 17.
 * https://oeis.org/A059425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime values of f(k) = k^2 + 19k + 17 (k = 0, 1, 2, …);
 *            a(n) is the nth prime among these f(k)
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Cache of the primes found so far. */
static seq_t *cache = NULL;         /* array of primes */
static size_t cached   = 0;         /* number of primes stored */
static size_t cap      = 0;         /* current cache capacity */
static idx_t next_k    = 0;         /* next k to test (starting from 0) */

/* Ensure the cache can hold at least 'new_cap' elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cap) return;
    seq_t *p = seq_realloc(cache, cap, new_cap);
    cache = p;
    cap   = new_cap;
}

/* Extend the cache until it contains at least 'target' primes. */
static void extend_cache(idx_t target)
{
    while (cached < target) {
        seq_t k = (seq_t)next_k;
        seq_t val = k * k + (seq_t)19 * k + (seq_t)17;
        if (is_prime_big(val)) {
            if (cached == cap)
                grow_cache(cap == 0 ? 8 : cap * 2);
            cache[cached++] = val;
        }
        ++next_k;
    }
}

/* Return the nth term of OEIS A059425 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */
    if (cached < n) extend_cache(n);
    return cache[n - 1];
}
