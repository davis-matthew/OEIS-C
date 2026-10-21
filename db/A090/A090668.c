/*
 * A090668  a(n) = floor( prime(n)*(n-pi(n))/(pi(n)+1) ) where pi=A000720.
 * https://oeis.org/A090668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor( prime(n) * (n - pi(n)) / (pi(n) + 1) ), where
 *            pi(n) is the prime counting function A000720
 * Range:     u8 n <= 11, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   upper_bound_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../utils/upper_bound_nth_prime.h"

/* Allocate memory for an array of uint32_t with dynamic resizing */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

/* Current upper bound that has been sieved */
static idx_t sieve_limit = 0;

/* Forward declarations */
static void ensure_primes_at_least(size_t n);
static size_t count_primes_leq(idx_t x);
static idx_t upper_bound_nth_prime(idx_t n);

/* Helper: Sieve of Eratosthenes up to 'limit', storing primes in prime_list. */
static void sieve_up_to(idx_t limit)
{
    char *is_composite = (char *)calloc((size_t)limit + 1, 1);
    if (!is_composite) return;

    prime_cap = prime_count = 0;
    size_t needed_cap = (size_t)((limit * 0.15) + 10); /* rough estimate */
    if (prime_cap < needed_cap) {
        prime_cap = needed_cap;
        prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
    }

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            /* store prime */
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_count++] = (uint32_t)i;

            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    free(is_composite);
}

/* Ensure that prime_list contains at least n primes (n >= 1). */
static void ensure_primes_at_least(size_t n)
{
    if (prime_count >= n) return;

    idx_t needed = upper_bound_nth_prime((idx_t)n);
    if (sieve_limit < needed) {
        sieve_up_to(needed);
        sieve_limit = needed;
    }
    /* In rare cases the bound may be too low; retry with a larger limit. */
    while (prime_count < n) {
        needed = needed * 2;
        sieve_up_to(needed);
        sieve_limit = needed;
    }
}

/* Count how many primes in prime_list are ≤ x. */
static size_t count_primes_leq(idx_t x)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((idx_t)prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* The sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but guard against underflows */
    /* Ensure required primes are available */
    ensure_primes_at_least((size_t)n);
    /* nth prime (1-indexed) */
    seq_t p_n = (seq_t)prime_list[n - 1];
    /* pi(n): number of primes ≤ n */
    size_t pi_n = count_primes_leq(n);
    seq_t numerator = p_n * (seq_t)(n - pi_n);
    seq_t denom     = (seq_t)(pi_n + 1);
    return numerator / denom;  /* integer division floors the result */
}
