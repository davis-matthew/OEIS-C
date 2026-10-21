/*
 * A091531  Primes p such that k = 2p is the smallest positive solution to the
 *          equation phi(p+k) = phi(p) + phi(k), where phi is Euler's totient
 *          function.
 * https://oeis.org/A091531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A091531
 *  a(n) = the n-th prime p > 3 for which k = 2p is the smallest
 *  positive integer satisfying
 *      φ(p + k) = φ(p) + φ(k),
 *  where φ is Euler's totient function.
 *
 *  The implementation precomputes φ for all integers up to
 *  4 000 000 and the primes up to that bound using a linear
 *  sieve (Euler's sieve).  All primes p with p > 3 and p ≤
 *  4 000 000/3 are tested; the first n such primes are returned
 *  in order.  The size of 4 000 000 guarantees that the first
 *  10 000 terms required for the tests can be found.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static bool initialized = false;
static uint32_t *phi = NULL;            /* φ values, index 0..LIMIT */
static uint32_t *primes = NULL;         /* list of primes */
static size_t   primes_len = 0;

/* constants */
#define LIMIT (4000000ull)               /* up to which φ is precomputed */
#define PRIME_CAP (300000)               /* enough for ~260k primes */
#define MAXP (LIMIT/3)                   /* largest p we can test fully */

static void init_sieve(void)
{
    /* allocate arrays */
    phi = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!phi) exit(1);                 /* abort on failure */

    primes = (uint32_t *)malloc(PRIME_CAP * sizeof(uint32_t));
    if (!primes) exit(1);

    /* linear sieve for φ and prime list */
    uint32_t *lp = (uint32_t *)calloc((LIMIT + 1), sizeof(uint32_t));
    if (!lp) exit(1);

    phi[1] = 1;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (lp[i] == 0) {                     /* i is prime */
            lp[i] = i;
            if (primes_len < PRIME_CAP)
                primes[primes_len++] = i;
            else
                primes_len--;                 /* guard overflow, should not happen */

            phi[i] = i - 1;
        }

        for (size_t j = 0; j < primes_len; ++j) {
            uint32_t p = primes[j];
            uint64_t ip = (uint64_t)i * p;
            if (ip > LIMIT) break;
            lp[ip] = p;
            if (i % p == 0) {
                phi[ip] = phi[i] * p;
                break;
            } else {
                phi[ip] = phi[i] * (p - 1);
            }
        }
    }
    free(lp);
    initialized = true;
}

/*--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined below offset */

    if (!initialized) init_sieve();

    idx_t found = 0;                       /* number of qualifying primes found */

    for (size_t idx = 0; idx < primes_len; ++idx) {
        uint32_t p = primes[idx];
        if (p <= 3u) continue;             /* skip trivial primes */
        if (p > MAXP) break;               /* 3p would exceed LIMIT */

        uint32_t phi_p = p - 1u;

        bool ok = true;
        /* check all k from 1 to 2p-1 */
        for (uint32_t k = 1u; k < 2u * p; ++k) {
            if (phi[p + k] == phi_p + phi[k]) {
                ok = false;                 /* smaller solution exists */
                break;
            }
        }

        if (ok) {
            ++found;
            if (found == n) {
                return (seq_t)p;
            }
        }
    }
    /* If we reach this point, the LIMIT was insufficient. */
    return (seq_t)0;
}
