/*
 * A074736  Goedel encoding of the prime factors of n, in increasing order and
 *          repeated according to multiplicity.
 * https://oeis.org/A074736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=1}^{k} prime(i)^{p_i}, where p_1 <= … <= p_k are the
 *            prime factors of n (with multiplicity) sorted in increasing
 *            order
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Cache of the first primes (1‑based indexing). */
static uint64_t *prime_cache = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;

/* Ensure prime_cache holds at least N primes. */
static void ensure_prime_count(size_t N)
{
    if (prime_count >= N)
        return;

    size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
    while (new_cap < N)
        new_cap *= 2;

    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp)
        abort();                       /* memory exhaustion */
    prime_cache = tmp;
    prime_capacity = new_cap;

    uint64_t start = prime_count ? prime_cache[prime_count - 1] + 1 : 2;
    for (uint64_t x = start; prime_count < N; ++x) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count && prime_cache[i] * prime_cache[i] <= x; ++i) {
            if (x % prime_cache[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            prime_cache[prime_count++] = x;
    }
}

/* Return the i‑th prime (1‑based). */
static uint64_t get_prime(size_t i)
{
    ensure_prime_count(i);
    return prime_cache[i - 1];
}

/* Factor n into its distinct prime factors with multiplicity,
   storing the factors in ascending order into out[64].
   Returns the number of factors stored. */
static size_t factor_prime_list(uint64_t n, uint64_t out[64])
{
    size_t idx = 0;
    for (size_t p_idx = 1; ; ++p_idx) {
        uint64_t p = get_prime(p_idx);
        if (p > n / p)   /* p*p > n */
            break;
        while (n % p == 0) {
            if (idx >= 64)
                abort();   /* safety guard */
            out[idx++] = p;
            n /= p;
        }
    }
    if (n > 1) {
        if (idx >= 64)
            abort();
        out[idx++] = n;
    }
    return idx;
}

/* Compute a(n) with external linkage. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;   /* a(1) = 1 */

    uint64_t m = (uint64_t)n;
    uint64_t factors[64];
    size_t k = factor_prime_list(m, factors);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < k; ++i) {
        uint64_t prime_of_i = get_prime(i + 1);           /* prime(1) = 2, prime(2) = 3, ... */
        seq_t base = (seq_t)prime_of_i;
        seq_t exponent = (seq_t)factors[i];
        seq_t term = big_pow(base, exponent);
        result = result * term;
    }
    return result;
}
