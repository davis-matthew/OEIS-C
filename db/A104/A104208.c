/*
 * A104208  Numbers of digits in p1^p2 where p2 > p1 and (p1, p2) are twin
 *          primes.
 * https://oeis.org/A104208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes a(n) = number of decimal digits of p1^p2, 
   where (p1, p2) is the n-th pair of twin primes with p2 > p1.
   The formula used: floor(p2 * log10(p1)) + 1.
   Only integer arithmetic is used on seq_t values; floating point is
   confined to the temporary approximation of the logarithm.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* ---------- static state for sieve and twin primes ---------- */
static bool   *prime_table = NULL;   /* prime flags, prime_table[i] is true ⇔ i is prime */
static uint64_t *twin1      = NULL;   /* lower prime of each twin pair */
static uint64_t *twin2      = NULL;   /* higher prime of each twin pair */
static size_t   sieve_limit = 0;      /* maximum number sieved so far */
static size_t   twin_count  = 0;      /* number of twin pairs stored */

/* ----- build or rebuild sieve up to 'limit' (inclusive) ----- */
static void init_sieve(size_t limit)
{
    /* if we already have a larger sieve, nothing to do */
    if (limit <= sieve_limit) return;

    /* allocate new prime table */
    free(prime_table);
    prime_table = (bool *)malloc((limit + 1) * sizeof(bool));
    if (prime_table == NULL) return;

    /* initialise all entries as prime, except 0 and 1 */
    for (size_t i = 0; i <= limit; ++i) prime_table[i] = true;
    prime_table[0] = prime_table[1] = false;

    /* classic sieve of Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (prime_table[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                prime_table[j] = false;
        }
    }

    /* count twin primes to size the arrays */
    size_t count = 0;
    for (size_t p = 3; p + 2 <= limit; ++p)
        if (prime_table[p] && prime_table[p + 2]) ++count;

    /* allocate twin prime arrays */
    free(twin1);
    free(twin2);
    twin1 = (uint64_t *)malloc(count * sizeof(uint64_t));
    twin2 = (uint64_t *)malloc(count * sizeof(uint64_t));

    /* store twin primes */
    size_t idx = 0;
    for (size_t p = 3; p + 2 <= limit; ++p) {
        if (prime_table[p] && prime_table[p + 2]) {
            twin1[idx] = p;
            twin2[idx] = p + 2;
            ++idx;
        }
    }

    twin_count  = count;
    sieve_limit = limit;
}

/* ------------------------------ a(n) ------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – undefined for n < 1 */

    /* ensure we have at least n twin primes */
    if (sieve_limit == 0) init_sieve(1000000);   /* start with 1e6 */

    while (twin_count < (size_t)n) {
        size_t new_limit = sieve_limit * 2;
        if (new_limit == 0) new_limit = 2000000;  /* first non‑zero limit */
        init_sieve(new_limit);
        if (new_limit > 200000000) break;          /* safety cap */
    }

    if (n > twin_count) return 0;   /* shouldn't happen for given limits */

    uint64_t p1 = twin1[n - 1];
    uint64_t p2 = twin2[n - 1];

    /* digits in p1^p2 : floor(p2 * log10(p1)) + 1  */
    long double lg  = log10l((long double)p1);
    long double val = (long double)p2 * lg;
    unsigned long long digits = (unsigned long long)(val + 1e-12L) + 1U;

    return (seq_t)digits;   /* cast to seq_t (works for all supported types) */
}
