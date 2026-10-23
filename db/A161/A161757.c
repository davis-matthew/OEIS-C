/*
 * A161757  a(n) = (prime(n))^2 - (nonprime(n))^2.
 * https://oeis.org/A161757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^2 - nonprime(n)^2
 * Range:     u8 n <= 8, u16 n <= 56, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* external types (supplied by the build) */
typedef uint64_t idx_t;  /* index type */

/* static storage for primes and nonprimes */
static uint64_t *prime_list   = NULL;
static size_t   prime_cap    = 0;
static size_t   prime_cnt    = 0;

static uint64_t *nonprime_list = NULL;
static size_t   nonprime_cap   = 0;
static size_t   nonprime_cnt   = 0;

static uint64_t next_candidate = 0;  /* number to test next */

/* extend the prime and nonprime lists so that they contain at least 'n' elements each */
static void extend_cache(idx_t n)
{
    while (prime_cnt < n || nonprime_cnt < n)
    {
        uint64_t m = next_candidate++;
        bool is_prime = true;

        /* trial division by known primes */
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = prime_list[i];
            if (p * p > m) break;
            if (m % p == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (m >= 2 && is_prime)
        {
            if (prime_cnt == prime_cap)
            {
                prime_cap = prime_cap ? prime_cap * 2 : 4;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = m;
        }
        else
        {
            if (nonprime_cnt == nonprime_cap)
            {
                nonprime_cap = nonprime_cap ? nonprime_cap * 2 : 4;
                nonprime_list = (uint64_t *)realloc(nonprime_list, nonprime_cap * sizeof(uint64_t));
            }
            nonprime_list[nonprime_cnt++] = m;
        }
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined for n = 0 */
    extend_cache(n);
    seq_t sp  = (seq_t)prime_list[n - 1];
    seq_t snp = (seq_t)nonprime_list[n - 1];
    return sp * sp - snp * snp;
}
