/*
 * A141930  Primes congruent to 4 mod 25.
 * https://oeis.org/A141930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 324, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141930: a(n) = the n-th prime ≡ 4 (mod 25) (OEIS offset 1).
   The function searches primes by sieving up to a dynamic upper bound. 
   It memoises the primes up to the largest n requested so far. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *prime_arr       = NULL;   /* primes ≡ 4 mod 25, stored as uint64_t */
static size_t   prime_len       = 0;      /* number of primes stored    */
static size_t   prime_cap       = 0;      /* allocated capacity         */

static char    *sieve          = NULL;    /* byte sieve buffer          */
static uint64_t sieve_limit    = 0;        /* current upper bound        */

/* Grow the prime array capacity to accommodate at least 'need' entries. */
static void grow_prime_array(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_arr = tmp;
    prime_cap = new_cap;
}

/* Build a sieve up to 'limit' and collect primes ≡ 4 mod 25 into prime_arr. */
static void build_sieve(uint64_t limit)
{
    /* Allocate or resize sieve buffer. */
    if (sieve) free(sieve);
    sieve = (char *)malloc((size_t)(limit + 1));
    if (!sieve) abort();
    memset(sieve, 1, (size_t)(limit + 1));
    sieve[0] = 0;
    if (limit >= 1) sieve[1] = 0;

    uint64_t i, j;
    for (i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (j = i * i; j <= limit; j += i) sieve[j] = 0;
        }
    }

    /* Rebuild prime array. */
    if (prime_arr) free(prime_arr);
    prime_arr   = NULL;
    prime_len   = 0;
    prime_cap   = 0;

    grow_prime_array(16);
    for (i = 2; i <= limit; ++i) {
        if (sieve[i] && (i % 25ULL) == 4ULL) {
            if (prime_len == prime_cap) grow_prime_array(prime_cap * 2);
            prime_arr[prime_len++] = i;
        }
    }
    sieve_limit = limit;
}

/* Rough upper bound for the n-th prime in this residue class. */
static uint64_t estimate_limit(idx_t n)
{
    return (uint64_t)n * 300ULL + 100ULL;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* behaviour below offset is irrelevant */

    /* Ensure enough primes are available. */
    while (prime_len < (size_t)n) {
        uint64_t new_limit;
        if (!sieve_limit) {
            new_limit = estimate_limit(n);
        } else {
            new_limit = sieve_limit * 2ULL;
        }
        if (new_limit <= sieve_limit) new_limit = sieve_limit * 2ULL;
        build_sieve(new_limit);
    }

    /* Convert stored 64‑bit prime to seq_t. This cast is safe because
       all primes produced fit in 64 bits for n up to 10 000. */
    return (seq_t)prime_arr[n - 1];
}
