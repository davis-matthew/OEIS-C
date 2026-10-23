/*
 * A142765  Primes congruent to 38 mod 59.
 * https://oeis.org/A142765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 112, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A142765: primes congruent to 38 modulo 59.
 * Implementation: sieve primes up to an estimated bound and
 * scan the arithmetic progression 59*k+38.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static bool *sieve = NULL;          /* primality bit array */
static idx_t sieve_limit = 0;       /* current sieve upper bound */

/* Estimate an upper bound for the nth prime in this progression.
   Uses PNT asymptotics for primes in a reduced residue class. */
static idx_t estimate_upper(idx_t n)
{
    if (n <= 1) return 200;                 /* safety for very small n */
    double dn = (double)n;
    double phi = 58.0;                      /* φ(59) = 58 */
    double lb  = log(dn * phi + 1.0);
    double est = dn * phi * (lb + 1.0);     /* n*φ*(log(nφ)+1) */
    return (idx_t)(est * 1.2) + 100;        /* extra safety margin */
}

/* Build or expand the sieve up to 'limit' (inclusive). */
static void build_sieve(idx_t limit)
{
    free(sieve);
    sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!sieve) return;                     /* allocation failure */
    for (idx_t i = 0; i <= limit; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    idx_t root = (idx_t)sqrt((double)limit);
    for (idx_t i = 2; i <= root; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = false;
        }
    }
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* only defined for n >= 1 */

    /* Ensure we have a sieve large enough. */
    idx_t lim = estimate_upper(n);
    if (lim > sieve_limit) build_sieve(lim);

    seq_t count = 0;
    for (idx_t val = 97; val <= sieve_limit; val += 59) {
        if (sieve[val]) {
            ++count;
            if (count == n) return (seq_t)val;
        }
    }

    /* If not found, enlarge the sieve and retry. */
    while (true) {
        idx_t new_lim = sieve_limit * 2;
        build_sieve(new_lim);
        for (idx_t val = 97; val <= sieve_limit; val += 59) {
            if (sieve[val]) {
                ++count;
                if (count == n) return (seq_t)val;
            }
        }
    }
}
