/*
 * A129781  The prime(n)-th upper twin prime.
 * https://oeis.org/A129781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A006512(A000040(n))
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the prime(n)-th upper twin prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t x); */

static idx_t *primes_arr      = NULL; /* array of primes */
static size_t primes_count    = 0;    /* how many primes we have */
static idx_t prime_candidate  = 2;    /* next integer to test for primality */

static idx_t *upper_twins_arr = NULL; /* array of upper twin primes */
static size_t upper_twins_count = 0;   /* how many upper twins we have */
static idx_t twin_candidate  = 3;      /* next odd to test for twin (p,p+2) */

/* Ensure that primes_arr contains at least `need` primes. */
static void ensure_primes(size_t need)
{
    if (primes_count >= need) return;
    size_t new_cap = primes_count ? (primes_count << 1) : 64;
    while (new_cap < need) new_cap <<= 1;
    idx_t *tmp = (idx_t*)realloc(primes_arr, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    primes_arr = tmp;

    while (primes_count < need) {
        if (is_prime(prime_candidate)) {
            primes_arr[primes_count] = prime_candidate;
            primes_count++;
        }
        prime_candidate++;
    }
}

/* Ensure that upper_twins_arr contains at least `need` elements. */
static void ensure_upper_twins(size_t need)
{
    if (upper_twins_count >= need) return;
    size_t new_cap = upper_twins_count ? (upper_twins_count << 1) : 64;
    while (new_cap < need) new_cap <<= 1;
    idx_t *tmp = (idx_t*)realloc(upper_twins_arr, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    upper_twins_arr = tmp;

    while (upper_twins_count < need) {
        if (is_prime(twin_candidate) && is_prime(twin_candidate + 2)) {
            upper_twins_arr[upper_twins_count] = twin_candidate + 2;
            upper_twins_count++;
        }
        twin_candidate += 2;
    }
}

seq_t a(idx_t n)
{
    /* Step 1: find the n‑th prime (1‑based). */
    if (n == 0) return (seq_t)0;            /* value for offset ≤0 is irrelevant */
    ensure_primes((size_t)n);
    idx_t prime_n = primes_arr[n - 1];

    /* Step 2: find the prime_n‑th upper twin prime. */
    ensure_upper_twins((size_t)prime_n);
    return (seq_t)upper_twins_arr[prime_n - 1];
}
