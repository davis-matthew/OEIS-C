/*
 * A079705  3p^2 where p runs through the primes.
 * https://oeis.org/A079705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3 * prime(n)^2 (prime(1)=2)
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is defined for n ≥ 1.  For n = 0 the function
    returns 0 – this value is never used in the tests.  */

#include <stdlib.h>
#include <stdint.h>

/* Dynamically maintained list of primes. */
static uint32_t *prime_list = NULL;   /* array of primes */
static size_t      prime_cap   = 0;   /* allocated size of prime_list */
static size_t      prime_cnt   = 0;   /* number of primes stored */

/* Ensure that prime_list contains at least 'need' primes. */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need)
        return;

    /* Grow the array if necessary. */
    if (prime_cap < need) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 128;
        if (new_cap < need)
            new_cap = need;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!tmp) {                /* allocation failed – terminate */
            abort();
        }
        prime_list = tmp;
        prime_cap  = new_cap;
    }

    /* Initialisation of the prime list. */
    if (prime_cnt == 0) {
        prime_list[prime_cnt++] = 2;
    }

    /* Generate further primes until we reach the required count. */
    uint32_t candidate = prime_list[prime_cnt - 1] + 1;
    while (prime_cnt < need) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            /* Stop when p*p would overflow or exceed the candidate. */
            if ((uint64_t)p * (uint64_t)p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            prime_list[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/*  Main routine required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1 – unused zero entry */
        return (seq_t)0;

    /* Prime list is 1‑indexed internally: prime_list[0] = prime(1) = 2. */
    ensure_primes((size_t)n);

    uint32_t p = prime_list[n - 1];
    seq_t pp = (seq_t)p * (seq_t)p;   /* p² */
    return (seq_t)3 * pp;             /* 3·p² */
}
