/*
 * A079276  Multiplicative inverse in the finite field F(prime(n)) of the
 *          product of the first n-1 primes modulo prime(n).
 * https://oeis.org/A079276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (product of the first n‑1 primes modulo prime(n))⁻¹ in
 *            F(prime(n)) (a(1)=1)
 * Range:     u8 n <= 54, u16 n <= 5025, u32 n <= 6903, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_inv_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/mod_inv_seq_t.h"

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_alloc = 0;
static size_t sieve_limit = 0;

/* Ensure that at least 'n' primes are available in the 'primes' array.
   The array is reallocated if necessary. */
static void ensure_primes(size_t n)
{
    if (primes_cnt >= n)
        return;
    /* Estimate upper bound for the nth prime.  For n >= 6,
       p_n < n (log n + log log n). */
    double nn = (double)n;
    size_t limit = (size_t)(nn * (log(nn) + log(log(nn)))) + 10;
    if (limit <= sieve_limit)
        limit = sieve_limit + 10;
    sieve_limit = limit;

    uint8_t *is_comp = (uint8_t *)calloc(limit + 1, 1);
    if (!is_comp) {
        /* Hard failure */
        exit(1);
    }

    /* Reallocate primes array if needed */
    if (primes_alloc < n) {
        uint32_t *tmp = (uint32_t *)realloc(primes, n * sizeof(uint32_t));
        if (!tmp) {
            free(is_comp);
            exit(1);
        }
        primes = tmp;
        primes_alloc = n;
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (cnt < n)
                primes[cnt++] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    primes_cnt = cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    ensure_primes((size_t)n);

    seq_t prime_n = (seq_t)primes[n - 1];
    seq_t prod = 1;
    for (size_t i = 0; i < (size_t)(n - 1); ++i) {
        prod = (prod * (seq_t)primes[i]) % prime_n;
    }

    return mod_inv_seq_t(prod, prime_n);
}
