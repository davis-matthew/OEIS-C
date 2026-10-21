/*
 * A096127  a(n) is the largest k such that (n^2)!/(n!)^k is an integer.
 * https://oeis.org/A096127
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{p <= n} floor(v_p((n^2)!)/v_p(n!)) (v_p is the
 *            exponent of the prime p in a factorial). The function builds
 *            prime tables on demand up to the largest n requested and uses
 *            Legendre's formula to compute the exponents
 * Range:     u8 n <= 143, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static prime table – rebuilt only when a larger n is requested. */
static uint64_t *prime_table = NULL;   /* dynamically allocated array of primes */
static size_t prime_count = 0;         /* number of primes stored */
static idx_t prime_limit = 0;          /* highest n for which the table is valid */

/* --------------------------------------------------------------- */
/* Update the prime table so that it contains all primes <= n.    */
/* Every time n is larger than the current limit we perform a    */
/* fresh simple sieve (sufficiently fast for n up to at least 10k). */
static void update_primes(idx_t n)
{
    if (n <= prime_limit) return;

    /* free previous table */
    if (prime_table) free(prime_table);

    /* allocate a temporary sieve array */
    /* use bool array to mark composites – indices 0..n inclusive */
    bool *sieve = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    if (!sieve) {                     /* allocation failed – abort */
        return;
    }

    size_t count = 0;
    for (idx_t i = 2; i <= n; ++i) sieve[i] = true;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (sieve[p]) {
            for (idx_t m = p * p; m <= n; m += p) sieve[m] = false;
        }
    }

    /* count primes to allocate exact array size */
    for (idx_t i = 2; i <= n; ++i) if (sieve[i]) ++count;
    prime_table = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!prime_table) {                 /* allocation failed – abort */
        free(sieve);
        return;
    }

    /* fill prime array */
    size_t pos = 0;
    for (idx_t i = 2; i <= n; ++i) if (sieve[i]) {
        prime_table[pos++] = (uint64_t)i;
    }

    prime_count = count;
    prime_limit  = n;
    free(sieve);
}

/* --------------------------------------------------------------- */
/* Legendre's formula: exponent of prime p in m! */
static idx_t legendre(idx_t m, idx_t p)
{
    idx_t exp = 0;
    idx_t power = p;
    while (power <= m) {
        exp += m / power;
        if (power > m / p) break;     /* avoid overflow of power *= p */
        power *= p;
    }
    return exp;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* undefined for n < 2 – return 0 */

    update_primes(n);                    /* ensure primes up to n are available */

    idx_t nn = n * n;                    /* n^2 */
    bool first = true;
    seq_t min_k = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = (idx_t)prime_table[i];
        if (p > n) break;                /* only primes <= n matter */

        idx_t v_num = legendre(nn, p);
        idx_t v_den = legendre(n,  p);
        if (v_den == 0) continue;        /* safety guard */

        idx_t k = v_num / v_den;
        seq_t k_seq = (seq_t)k;
        if (first || k_seq < min_k) {
            min_k = k_seq;
            first = false;
        }
    }
    return min_k;
}
