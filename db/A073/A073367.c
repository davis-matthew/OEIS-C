/*
 * A073367  Remainder when n-th composite is divided by number of composites
 *          not exceeding n.
 * https://oeis.org/A073367
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002808(n) mod A065855(n) Where A002808(n) is the n-th
 *            composite number and A065855(n) = n - π(n) - 1 (number of
 *            composites ≤ n)
 * Range:     u8 n <= 641, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*--------------------------------------------------------------*/
/*  Sieve data structures and construction                      */
/*--------------------------------------------------------------*/
static uint8_t  *is_prime    = NULL;   /* 1 = prime, 0 = composite */
static uint32_t *prime_list  = NULL;   /* list of primes */
static size_t    sieve_limit = 0;      /* current maximum index */
static size_t    prime_count = 0;      /* number of primes stored */
static size_t    prime_capacity = 0;   /* allocated capacity */

/* Expand the sieve to cover all integers up to at least 'limit'. */
static void init_sieve(size_t limit)
{
    if (limit <= sieve_limit)          /* nothing to do */
        return;

    if (limit < 4) limit = 4;           /* 0…3 handled explicitly */

    /* Grow the prime‑mark array.  Newly allocated entries are assumed prime. */
    is_prime = (uint8_t *)realloc(is_prime, (limit + 1) * sizeof(uint8_t));
    if (!is_prime)
        return;                       /* allocation failure */
    memset(&is_prime[sieve_limit + 1], 1,
           (limit - sieve_limit) * sizeof(uint8_t));
    is_prime[0] = 0; is_prime[1] = 0;  /* 0 and 1 are not prime */

    /* Perform Eratosthenes sieve up to 'limit'. */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count new prime occurrences in the extended range. */
    size_t new_primes = 0;
    for (size_t i = sieve_limit + 1; i <= limit; ++i)
        if (is_prime[i])
            new_primes++;

    /* Grow the prime list buffer if necessary. */
    if (prime_count + new_primes > prime_capacity) {
        prime_capacity = (prime_count + new_primes) * 2 + 10;
        prime_list = (uint32_t *)realloc(prime_list,
                                         prime_capacity * sizeof(uint32_t));
        if (!prime_list)
            return;                    /* allocation failure */
    }

    /* Append new primes to the list. */
    for (size_t i = sieve_limit + 1; i <= limit; ++i) {
        if (is_prime[i])
            prime_list[prime_count++] = (uint32_t)i;
    }

    sieve_limit = limit;
}

/*--------------------------------------------------------------*/
/*  Auxiliary functions                                         */
/*--------------------------------------------------------------*/
/* Return π(n), the number of primes ≤ n. */
static idx_t count_primes_upto(idx_t n)
{
    if ((size_t)n > sieve_limit)
        init_sieve((size_t)n);

    /* Binary search in prime_list for first prime > n. */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)lo;
}

/* Return the n-th composite number (n ≥ 1). */
static idx_t composite_n(idx_t n)
{
    if (n == 0) return 0;              /* not used for valid inputs */

    /* Rough upper bound sufficient for n up to 10 000. */
    size_t bound = (n < 5) ? 10 : 2 * n + 10;
    init_sieve(bound);

    size_t cnt = 0;
    for (idx_t val = 4; ; ++val) {
        if ((size_t)val > sieve_limit) {
            size_t new_limit = (sieve_limit * 2 > val + 10) ? sieve_limit * 2
                                                             : val + 10;
            init_sieve(new_limit);
        }
        if (!is_prime[(size_t)val]) {
            ++cnt;
            if (cnt == n)
                return val;
        }
    }
}

/*--------------------------------------------------------------*/
/*  Required sequence function                                 */
/*--------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 4) return 0;               /* behaviour for n < 4 is unspecified */

    /* number of composites not exceeding n:  n – π(n) – 1 */
    idx_t comp_cnt = n - count_primes_upto(n) - 1;

    seq_t comp_val = (seq_t)composite_n(n);

    /* remainder when comp_val is divided by comp_cnt */
    return (seq_t)(comp_val % comp_cnt);
}
