/*
 * A082666  a(1) = 0, for n > 1 if n = prime(k), then a(n) = k, otherwise a(n)
 *          = Sum_{d|n, d<n} a(d).
 * https://oeis.org/A082666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, for n>1, a(n)=A049084(n)+∑_{d|n , d<n} a(d)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=0; a(n)=index of n if n is prime, otherwise sum of a(d) for all proper divisors d of n. */

#include <stdlib.h>
#include <stdbool.h>

/* Cached values of the sequence, 1‑based.  a_cache[0] is unused. */
static seq_t *a_cache = NULL;
/* List of primes found so far, 1‑based index. */
static idx_t *primes_list = NULL;
/* Number of primes collected. */
static size_t primes_count = 0;
/* Highest computed index.  a_cache[1] is always set to 0. */
static idx_t last_computed = 1;

/* Ensure the cache can hold values up to n.  Zero‑initialises a_cache[1]. */
static void ensure_cache_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    size_t current = a_cache ? (size_t)(last_computed + 1) : 0;
    if (needed <= current) return;

    size_t new_size = current > 0 ? current : 2;
    while (new_size < needed) new_size <<= 1;

    seq_t *new_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    a_cache = new_cache;
    /* At least a_cache[1] must be defined. */
    a_cache[1] = 0;
}

/* Ensure the prime list has space for at least p elements. */
static void ensure_primes_capacity(size_t p)
{
    size_t needed = p + 1;
    size_t current = primes_list ? (primes_count + 1) : 0;
    if (needed <= current) return;

    size_t new_size = current > 0 ? current : 16;
    while (new_size < needed) new_size <<= 1;

    idx_t *new_list = (idx_t *)realloc(primes_list, new_size * sizeof(idx_t));
    if (!new_list) abort();
    primes_list = new_list;
}

/* Test primality using the stored primes. */
static bool is_prime(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_count; ++i) {
        idx_t p = primes_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of bounds guard */
    if (n == 1) return 0;          /* a(1) = 0 */

    if (n > last_computed) {
        ensure_cache_capacity(n);
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    if (d < i) sum += a_cache[d];
                    idx_t other = i / d;
                    if (other != d && other < i) sum += a_cache[other];
                }
            }

            seq_t prime_index = 0;
            if (is_prime(i)) {
                prime_index = (seq_t)(primes_count + 1);
                ensure_primes_capacity(primes_count + 1);
                primes_list[primes_count++] = i;
            }

            a_cache[i] = sum + prime_index;
        }
        last_computed = n;
    }

    return a_cache[n];
}
