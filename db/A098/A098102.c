/*
 * A098102  a(n) = 2^(prime(n) - 1) - 1 where prime(n) is the n-th prime.
 * https://oeis.org/A098102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(prime(n) - 1) - 1 , with prime(n) the n‑th prime
 *            (offset 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/*  Helper: extend the list of primes until we have at least n primes.
    The implementation uses a simple sieve of Eratosthenes and is
    re‑computable with a larger bound if more primes are needed.   */
static uint64_t *prime_list = NULL;      /* array of primes */
static size_t prime_cap   = 0;           /* capacity of prime_list */
static size_t prime_cnt   = 0;           /* number of primes stored */

static void grow_primes(size_t needed)
{
    /* Estimate an upper bound for the needed n‑th prime.
       For n >= 6, p_n < n(ln n + ln ln n).  We use a coarse but safe
       bound  n * 20  for the sizes we handle (~10000).               */
    size_t new_cap = prime_cap ? prime_cap : 64;
    while (prime_cnt < needed) {
        /* increase bound until we can hold the required number of primes */
        size_t limit = (prime_cnt ? new_cap * 2 : 256);
        /* allocate is_prime array */
        char *is_prime = (char *)malloc(limit + 1);
        for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (size_t i = 2; i * i <= limit; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0;
        /* rebuild prime list up to new limit */
        size_t new_primes = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (is_prime[i]) ++new_primes;
        /* allocate new array */
        uint64_t *new_list = (uint64_t *)malloc(new_primes * sizeof(uint64_t));
        size_t idx = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (is_prime[i]) new_list[idx++] = i;
        }
        free(prime_list);
        free(is_prime);
        prime_list = new_list;
        prime_cap   = new_primes;
        prime_cnt   = new_primes;
        new_cap = prime_cap;
    }
}

/* Return the nth prime (1‑based indexing). */
static uint64_t nth_prime(idx_t n)
{
    if (prime_cnt < (size_t)n) grow_primes((size_t)n);
    return prime_list[(size_t)n - 1];
}

/* Compute the sequence term a(n) = 2^(prime(n)-1) - 1. */
seq_t a(idx_t n)
{
    uint64_t p = nth_prime(n);
    /* For seq_t that is (arbitrary‑precision or 128‑bit) the shift is
       defined for the whole range.  For 64‑bit builds this will only
       be called with p-1 < 64 because otherwise the result does not
       fit in seq_t and will not be verified.                      */
    seq_t val = ((seq_t)1 << (p - 1)) - 1;
    return val;
}
