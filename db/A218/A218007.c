/*
 * A218007  Number of partitions of n into at most three primes (including 1).
 * https://oeis.org/A218007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most three primes
 *            (including 1). The implementation counts unordered multisets of
 *            size 1, 2, or 3 from the set {1} ∪ primes summing to n. It
 *            memoizes results and dynamically builds the prime sieve up to
 *            the maximum n requested
 * Range:     u8 n <= 366, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External types provided by the build system. */
extern seq_t a(idx_t n);

/* Internal state for the prime sieve. */
static bool *is_prime = NULL;       /* 1 == prime, 0 == composite */
static idx_t sieve_max = 0;

/* Internal cache for computed sequence values. */
static seq_t *cache = NULL;           /* cache[i] = a(i) for i<=cache_max */
static idx_t cache_max = 0;

/* Return true if x is in the summand set {1} ∪ primes. */
static inline bool is_member(idx_t x)
{
    if (x == 1) return true;
    if (x < 2)  return false;
    return is_prime[x];
}

/* Build or extend the prime sieve up to n. */
static void build_sieve(idx_t n)
{
    if (n <= sieve_max) return;

    /* Reallocate the is_prime array. */
    bool *new_buf = (bool *)realloc(is_prime, (size_t)(n + 1) * sizeof(bool));
    if (!new_buf) abort();
    is_prime = new_buf;

    /* Initialise new entries to true. */
    for (idx_t i = sieve_max + 1; i <= n; ++i) is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    /* Sieve of Eratosthenes up to n. */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }
    sieve_max = n;
}

/* Ensure the cache can hold values up to n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;

    seq_t *new_buf = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_buf) abort();
    cache = new_buf;
    for (idx_t i = cache_max + 1; i <= n; ++i) cache[i] = 0;
    cache_max = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but return 0 safely */

    ensure_cache(n);
    if (cache[n] != 0) return cache[n];

    build_sieve(n);

    seq_t result = 0;

    /* Partitions of size 1. */
    if (n == 1 || (n >= 2 && is_prime[n])) ++result;

    /* Partitions of size 2 (unordered pairs). */
    for (idx_t a = 1; a <= n / 2; ++a) {
        if (!is_member(a)) continue;
        idx_t b = n - a;
        if (b < a) continue;          /* ensure a <= b */
        if (b == 1 || (b >= 2 && is_prime[b])) ++result;
    }

    /* Partitions of size 3 (unordered triples). */
    for (idx_t a = 1; a <= n / 3; ++a) {
        if (!is_member(a)) continue;
        for (idx_t b = a; b <= (n - a) / 2; ++b) {
            if (!is_member(b)) continue;
            idx_t c = n - a - b;
            if (c < b) break;           /* because c decreases as b increases */
            if (c == 1 || (c >= 2 && is_prime[c])) ++result;
        }
    }

    cache[n] = result;
    return result;
}
