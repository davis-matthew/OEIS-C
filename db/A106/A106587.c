/*
 * A106587  Sum of n-th prime squared and n-th perfect square.
 * https://oeis.org/A106587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 + prime(n)^2
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* dynamic storage for the prime table */
static uint32_t *prime_list = NULL;
static size_t     prime_count = 0;
static size_t     prime_cap   = 0;
static uint32_t   max_checked = 0;

/* Generate primes until we have at least n primes.
   The routine uses trial division with the primes already found. */
static void generate_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;

    /* first initialisation of the table */
    if (!prime_list) {
        prime_cap = 16;
        prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_list)       /* allocation failure – give up silently */
            return;
        prime_list[0] = 2;
        prime_count   = 1;
        max_checked   = 2;
    }

    uint32_t candidate = max_checked + 1;

    while (prime_count < (size_t)n) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            /* stop if p^2 > candidate */
            if ((uint64_t)p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp)       /* allocation failure – give up silently */
                    return;
                prime_list = tmp;
            }
            prime_list[prime_count++] = candidate;
        }

        max_checked = candidate;
        ++candidate;
    }
}

/* Public sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) is undefined in the OEIS data */

    generate_primes(n);

    /* compute prime(n)^2 + n^2 with only integer arithmetic */
    seq_t p   = (seq_t)prime_list[(size_t)n - 1];
    seq_t p2  = p * p;
    seq_t n2  = (seq_t)n * (seq_t)n;
    return p2 + n2;
}
