/*
 * A068227  The "genity" sequence of the primes, i.e., a(n) = g(p) = ((p mod
 *          4) + (p mod 6))/2, where p is the n-th prime.
 * https://oeis.org/A068227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((p_n mod 4) + (p_n mod 6)) / 2, where p_n is the n‑th
 *            prime (n ≥ 1)
 * Range:     u8 n <= 54, u16 n <= 7159, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for generated primes */
static uint32_t *primes = NULL;          /* array of primes */
static size_t primes_len  = 0;           /* number of primes stored */
static size_t primes_cap  = 0;           /* current capacity of the array */

/* ensures that at least n primes are generated; n ≥ 1 */
static void ensure_primes(idx_t n)
{
    if (primes_len >= n)
        return;

    /* initialise storage */
    if (!primes) {
        primes_cap  = 128;
        primes      = (uint32_t *)malloc(sizeof(uint32_t) * primes_cap);
        primes_len  = 0;
    }

    /* first prime */
    if (primes_len == 0) {
        primes[0] = 2;
        primes_len = 1;
    }

    /* generate further primes via trial division */
    for (uint32_t candidate = primes[primes_len - 1] + 1;
         primes_len < n; ++candidate) {

        bool prime = true;
        /* trial division by known primes up to sqrt(candidate) */
        for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= candidate; ++i) {
            if (candidate % primes[i] == 0) {
                prime = false;
                break;
            }
        }

        if (prime) {
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                primes = (uint32_t *)realloc(primes, sizeof(uint32_t) * primes_cap);
            }
            primes[primes_len++] = candidate;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    ensure_primes(n);
    uint32_t p = primes[n - 1];      /* primes are stored 0‑based, n is 1‑based */

    seq_t r4 = (seq_t)p % (seq_t)4;
    seq_t r6 = (seq_t)p % (seq_t)6;
    return (r4 + r6) / (seq_t)2;
}
