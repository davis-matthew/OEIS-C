/*
 * A048851  Length of hypotenuse squared in right triangle formed by a prime
 *          spiral plotted in Cartesian coordinates.
 * https://oeis.org/A048851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8 for n = 1; for n > 1, a(n) = p_{n-1}^2 + p_n^2 where
 *            p_k is the k‑th prime (p_1 = 2)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary standard headers */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static data to cache primes */
static uint64_t *prime_cache      = NULL;   /* pointers to found primes   */
static size_t   prime_cap       = 0;        /* allocated capacity        */
static size_t   prime_count     = 0;        /* number of primes stored   */

/* Grow the prime cache array to hold at least 'need' primes */
static void _grow_prime_cache(size_t need)
{
    if (prime_cap >= need)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp)
        abort();          /* allocation failure */
    prime_cache = tmp;
    prime_cap   = new_cap;
}

/* Ensure that 'need' primes are present in prime_cache. */
static void _ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;

    _grow_prime_cache(need);

    uint64_t candidate = (prime_count == 0) ? 2 : prime_cache[prime_count - 1] + 1;
    while (prime_count < need) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime)
            prime_cache[prime_count++] = candidate;
        ++candidate;
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)8;

    /* Need at least n primes (for p_{n-1} and p_n) */
    size_t need = (size_t)n;
    _ensure_primes(need);

    /* Retrieve the necessary primes */
    uint64_t p_prev = prime_cache[(size_t)n - 2];
    uint64_t p_curr = prime_cache[(size_t)n - 1];

    /* Compute p_prev^2 + p_curr^2 in seq_t arithmetic */
    seq_t pp_prev = (seq_t)p_prev;
    seq_t pp_curr = (seq_t)p_curr;
    return pp_prev * pp_prev + pp_curr * pp_curr;
}
