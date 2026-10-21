/*
 * A073846  a(1) = 1; thereafter, every even-indexed term is prime and every
 *          odd-indexed term is composite.
 * https://oeis.org/A073846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A073846:
   a(1) = 1;
   for n > 1:
      if n is even, a(n) = the (n/2)-th prime (prime(1)=2);
      if n is odd,  a(n) = the floor(n/2)-th composite
                      (composite(1)=4, composite(2)=6, …)      */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>     /* for sqrt() on both C and C++ */

/* internal static data */
static bool *is_prime_arr = NULL;   /* sieve flags, indices 0 .. sieve_limit */
static size_t sieve_limit = 0;      /* current maximum value sieved */

static uint32_t *prime_list = NULL;    /* primes up to sieve_limit */
static size_t   prime_count = 0;

static uint32_t *composite_list = NULL;/* composites ≥4 up to sieve_limit */
static size_t   composite_count = 0;

/* helper: rebuild the sieve and the prime/composite tables */
static void rebuild_sieve(size_t limit)
{
    /* free previous data */
    free(is_prime_arr);
    free(prime_list);
    free(composite_list);

    sieve_limit = limit;
    is_prime_arr = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime_arr) abort();

    /* initialise: 0 and 1 are not prime */
    if (limit >= 0) is_prime_arr[0] = false;
    if (limit >= 1) is_prime_arr[1] = false;
    for (size_t i = 2; i <= limit; ++i) is_prime_arr[i] = true;

    /* Sieve of Eratosthenes */
    size_t sqrt_lim = (size_t)sqrt((double)limit);
    for (size_t p = 2; p <= sqrt_lim; ++p)
        if (is_prime_arr[p])
            for (size_t q = p * p; q <= limit; q += p)
                is_prime_arr[q] = false;

    /* Count primes and composites */
    prime_count = 0;
    composite_count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime_arr[i]) ++prime_count;
        else if (i >= 4) ++composite_count;
    }

    /* Allocate and fill prime list */
    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime_arr[i]) prime_list[idx++] = (uint32_t)i;

    /* Allocate and fill composite list (starting from 4) */
    composite_list = (uint32_t *)malloc(composite_count * sizeof(uint32_t));
    if (!composite_list) abort();
    idx = 0;
    for (size_t i = 4; i <= limit; ++i)
        if (!is_prime_arr[i]) composite_list[idx++] = (uint32_t)i;
}

/* retrieve k-th prime (k >= 1) */
static uint32_t get_kth_prime(idx_t k)
{
    if (k == 0) return 0;          /* not used */
    while (prime_count < (size_t)k) {
        size_t new_limit = (sieve_limit == 0) ? 15 : sieve_limit * 2;
        rebuild_sieve(new_limit);
    }
    return prime_list[k - 1];
}

/* retrieve k-th composite (k >= 1) */
static uint32_t get_kth_composite(idx_t k)
{
    if (k == 0) return 0;          /* not used */
    while (composite_count < (size_t)k) {
        size_t new_limit = (sieve_limit == 0) ? 50 : sieve_limit * 2;
        rebuild_sieve(new_limit);
    }
    return composite_list[k - 1];
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    idx_t k = n / 2;           /* floor(n/2) */
    if ((n & 1) == 0)          /* even index → prime */
        return (seq_t)get_kth_prime(k);
    else                       /* odd index → composite */
        return (seq_t)get_kth_composite(k);
}
