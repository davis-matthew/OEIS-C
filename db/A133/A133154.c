/*
 * A133154  a(n) is the smallest m<=p-1 such that p-1 is the only value of j
 *          in 1<=j<=2p for which m^j+j==0 (mod p), where p is the n-th prime.
 * https://oeis.org/A133154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest m <= p-1 such that p-1 is the only j * (1 <=
 *            j <= 2p) for which m^j + j == 0 (mod p), where p is the n-th
 *            prime
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   compute_for_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_for_prime.h"

/* The build supplies the types seq_t and idx_t; we use them as is. */

/* Limits for a fast prime sieve.  The 10 000th prime is 104 729. */
#define SIEVE_LIMIT 200000
#define MAX_PRIMES  25000      /* safely covers the 10 000th prime */

static bool primes_built = false;
static uint32_t primes[MAX_PRIMES];
static size_t primes_cnt = 0;

/* Build the first MAX_PRIMES primes using a simple sieve.  */
static void build_primes(void)
{
    if (primes_built) return;

    bool *is_comp = (bool*)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    for (uint32_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                is_comp[j] = true;
            }
        }
    }

    for (uint32_t i = 2; i <= SIEVE_LIMIT && primes_cnt < MAX_PRIMES; ++i) {
        if (!is_comp[i]) primes[primes_cnt++] = i;
    }
    free(is_comp);
    primes_built = true;
}

/* Public interface required by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    build_primes();
    if ((size_t)n > primes_cnt) return (seq_t)0;

    uint32_t prime = primes[n - 1];
    return compute_for_prime(prime);
}
