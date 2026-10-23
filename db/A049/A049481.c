/*
 * A049481  Primes p such that p + 30 is also prime.
 * https://oeis.org/A049481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 2354, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generating primes p such that p+30 is also prime (A049481). */
/* The function uses a simple incremental sieve: it doubles the sieving
   limit until the required number of special primes is found.  The result
   is cached so subsequent calls are O(1).  Only standard C11 and
   implicitly available helpers are used. */

/* External integer types are defined by the build environment. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Static state used to build the sieve and cache the results. */
static uint8_t *is_prime = NULL;      /* sieve bitmap */
static uint64_t *prime_list = NULL;   /* list of primes up to current limit */
static size_t prime_cnt = 0;          /* number of primes in prime_list */
static seq_t *special_list = NULL;    /* list of primes p with p+30 prime */
static size_t special_cnt = 0;        /* number of such primes */
static size_t sieve_limit = 0;        /* current upper bound of the sieve */

/* Rebuild the sieve until at least 'need' special primes are found. */
static void rebuild_sieve(size_t need)
{
    /* Repeat until we have enough special primes. */
    while (special_cnt < need)
    {
        /* Increase the sieve limit (start with 1 000 000, then double). */
        size_t new_limit = (sieve_limit == 0) ? 1000000ULL : sieve_limit << 1;
        /* Avoid overflow when testing p+30. */
        if (new_limit > SIZE_MAX - 30) new_limit = SIZE_MAX - 1;

        /* Rebuild the prime bitmap. */
        free(is_prime);
        is_prime = (uint8_t *)malloc(new_limit + 1);
        if (!is_prime) abort();
        memset(is_prime, 1, new_limit + 1);
        is_prime[0] = 0;
        is_prime[1] = 0;

        /* Eratosthenes sieve on the bitmap. */
        for (size_t i = 2; i * i <= new_limit; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= new_limit; j += i)
                    is_prime[j] = 0;

        /* Build list of all primes up to new_limit. */
        free(prime_list);
        size_t prime_cap = new_limit / 10 + 100;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) abort();
        prime_cnt = 0;
        for (size_t i = 2; i <= new_limit; ++i)
            if (is_prime[i])
            {
                if (prime_cnt == prime_cap)
                {
                    prime_cap <<= 1;
                    prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
                    if (!prime_list) abort();
                }
                prime_list[prime_cnt++] = i;
            }

        /* Build list of special primes (p and p+30 both prime). */
        free(special_list);
        size_t special_cap = new_limit / 20 + 50;
        special_list = (seq_t *)malloc(special_cap * sizeof(seq_t));
        if (!special_list) abort();
        special_cnt = 0;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = prime_list[i];
            uint64_t q = p + 30;
            if (q <= new_limit && is_prime[q])
            {
                if (special_cnt == special_cap)
                {
                    special_cap <<= 1;
                    special_list = (seq_t *)realloc(special_list, special_cap * sizeof(seq_t));
                    if (!special_list) abort();
                }
                special_list[special_cnt++] = (seq_t)p;
            }
        }

        sieve_limit = new_limit;
    }
}

/* External function for the sequence.  It is 1‑based: a(1)=7. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined but harmless */
    rebuild_sieve((size_t)n);
    return special_list[n - 1];
}
