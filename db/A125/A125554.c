/*
 * A125554  a(0)=2. a(n) = the a(n-1)th integer from among those positive
 *          integers coprime to n.
 * https://oeis.org/A125554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=2; for n>0, a(n) = the a(n‑1)th positive integer that is
 *            coprime to n. Computation is done by binary search on the count
 *            of coprime numbers using inclusion–exclusion
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(n) must always be >0; 0 is used as a sentinel in the cache. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- cache of computed a(n) values ---------- */
static seq_t *a_cache = NULL;   /* a_cache[i] holds a(i); 0 means not computed */
static size_t a_cache_cap = 0;

/* Ensure the cache covers indices up to n */
static void ensure_a_cache(idx_t n)
{
    if (n < a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    a_cache = tmp;
    for (size_t i = a_cache_cap; i < new_cap; ++i) a_cache[i] = 0;
    a_cache_cap = new_cap;
}

/* ---------- factor n into distinct prime factors ---------- */
static size_t distinct_prime_factors(idx_t n, uint64_t *primes)
{
    size_t count = 0;
    uint64_t nn = (uint64_t)n;
    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            primes[count++] = p;
            while (nn % p == 0) nn /= p;
        }
    }
    if (nn > 1) primes[count++] = nn;
    return count;
}

/* ---------- count of integers <= x that are coprime to n ---------- */
static seq_t count_coprime(idx_t n, seq_t x)
{
    uint64_t primes[16];
    size_t m = distinct_prime_factors(n, primes);

    seq_t result = x;                     /* start with all numbers */
    size_t max_mask = 1u << m;
    for (size_t mask = 1; mask < max_mask; ++mask) {
        uint64_t prod = 1;
        size_t bits = 0;
        for (size_t i = 0; i < m; ++i) {
            if (mask & (1u << i)) {
                prod *= primes[i];
                ++bits;
            }
        }
        seq_t term = x / (seq_t)prod;
        if (bits & 1) result -= term;     /* subtract odd intersections */
        else          result += term;     /* add even intersections */
    }
    return result;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;

    ensure_a_cache(n);
    if (a_cache[n] != 0) return a_cache[n];

    seq_t prev = a(n - 1);      /* a(n‑1) is guaranteed >0 */

    /* factor n and compute φ(n) */
    uint64_t primes[16];
    size_t m = distinct_prime_factors(n, primes);
    uint64_t phi_n = (uint64_t)n;
    for (size_t i = 0; i < m; ++i) {
        uint64_t p = primes[i];
        phi_n = phi_n / p * (p - 1);
    }

    /* Estimate high bound: k/n * n + n (k = prev) */
    seq_t high = ((prev / (seq_t)phi_n) + (seq_t)1) * (seq_t)n + (seq_t)n;
    seq_t low = 1;

    while (low < high) {
        seq_t mid = low + (high - low) / 2;
        seq_t cnt = count_coprime(n, mid);
        if (cnt >= prev) high = mid;
        else             low  = mid + 1;
    }

    a_cache[n] = low;
    return low;
}
