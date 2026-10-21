/*
 * A056596  Number of nonsquare divisors of n!.
 * https://oeis.org/A056596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n!) - A046951(n!) * d(n!) = ∏_{p≤n} (e_p + 1) *
 *            A046951(n!) = ∏_{p≤n} (⌊e_p/2⌋ + 1) * where e_p is the exponent
 *            of prime p in n! (Legendre’s formula)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 49, u64 n <= 118,
 *            u128 n <= 282, big any n
 * Helpers:   legendre_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/legendre_exponent.h"

/* Static sieve data */
static char *prime_sieve = NULL; /* 1 = prime, 0 = composite */
static idx_t sieve_limit = 0;

/* Build a prime sieve up to 'limit' (inclusive). */
static void build_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;            /* already built up to at least 'limit' */
    if (prime_sieve) free(prime_sieve);          /* discard old sieve if any */
    prime_sieve = (char *)malloc(limit + 1);
    if (!prime_sieve) return;                    /* memory failure – callers will fail */

    for (idx_t i = 0; i <= limit; ++i) prime_sieve[i] = 1;
    if (limit >= 0) prime_sieve[0] = 0;
    if (limit >= 1) prime_sieve[1] = 0;

    for (idx_t i = 2; i <= limit / i; ++i) {
        if (prime_sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_sieve[j] = 0;
        }
    }
    sieve_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Offset is 1; return 0 for n=0 for safety */

    build_sieve(n);

    seq_t total = 1;                /* d(n!)            */
    seq_t sqdiv = 1;                /* square divisors */

    for (idx_t p = 2; p <= n; ++p) {
        if (!prime_sieve[p]) continue;  /* skip composites */

        idx_t e = legendre_exponent(n, p);   /* exponent of p in n! */
        total = total * (seq_t)(e + 1);
        sqdiv = sqdiv * (seq_t)(e / 2 + 1);
    }

    return total - sqdiv;
}
