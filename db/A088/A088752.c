/*
 * A088752  Let CompositePi(n) = number of composite numbers <= n and
 *          PrimePi(n) = number of primes <= n, so that CompositePi(n) +
 *          PrimePi(n) = n. Then a(n) = floor(n*(n -
 *          PrimePi(n))/(n-1-PrimePi(n-1) + n - 2 - PrimePi(n-2))).
 * https://oeis.org/A088752
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088752:
   a(n) = floor( n * (n - PrimePi(n)) /
               ( (n-1-PrimePi(n-1)) + (n-2-PrimePi(n-2)) ) )
   Offset: 3 (a(3) is the first term of the sequence)            */

#include <stdint.h>
#include <stdlib.h>

/* ---------- Prime counting cache ---------- */
static uint8_t *sieve_flags = NULL;   /* 1 = prime, 0 = composite */
static idx_t *prime_pi    = NULL;     /* prefix counts of primes */
static idx_t sieve_limit  = 1;        /* largest value for which sieve was built */

/* Build or extend a simple Eratosthenes sieve up to 'limit'.
   The arrays are reallocated if a larger limit is requested. */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* Free old tables if they exist */
    if (sieve_flags) free(sieve_flags);
    if (prime_pi)    free(prime_pi);

    /* Allocate new tables */
    sieve_flags = (uint8_t*)malloc((size_t)(limit + 1) * sizeof(uint8_t));
    prime_pi    = (idx_t*)malloc((size_t)(limit + 1) * sizeof(idx_t));

    /* Initialize sieve */
    if (limit >= 0) sieve_flags[0] = 0;
    if (limit >= 1) sieve_flags[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) sieve_flags[i] = 1;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (sieve_flags[p]) {
            for (idx_t x = p * p; x <= limit; x += p) sieve_flags[x] = 0;
        }
    }

    /* Build prefix array of prime counts */
    idx_t count = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (sieve_flags[i]) ++count;
        prime_pi[i] = count;
    }

    sieve_limit = limit;
}

/* ---------- Sequence term calculation ---------- */
seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* the sequence is defined for n >= 3 */

    /* Ensure we have all prime counts up to 'n' */
    ensure_sieve(n);

    /* Composite counts C(k) = k - PrimePi(k) */
    seq_t Cn  = (seq_t)n  - (seq_t)prime_pi[n];
    seq_t Cn1 = (seq_t)(n - 1) - (seq_t)prime_pi[n - 1];
    seq_t Cn2 = (seq_t)(n - 2) - (seq_t)prime_pi[n - 2];

    seq_t denom = Cn1 + Cn2;
    if (denom == 0) return 0;  /* safety – should not occur for n >= 3 */

    seq_t numer = (seq_t)n * Cn;
    return numer / denom;      /* floor division */
}
