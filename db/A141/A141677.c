/*
 * A141677  Number of divisors of n times the number of primes <= n.
 * https://oeis.org/A141677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(n)*A000720(n) (number of divisors of n multiplied
 *            by the number of primes <= n)
 * Range:     u8 n <= 83, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;  /* array of primes up to current limit */
static size_t primes_cnt = 0;     /* number of primes stored */
static idx_t primes_limit = 0;   /* highest n for which primes are computed */

/* Compute number of divisors of n using the primes array that is at least
   as large as sqrt(n).  This routine is small, fast, and avoids
   external helper functions. */
static seq_t divisor_count(idx_t n)
{
    seq_t d = 1;
    idx_t temp = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            d *= (seq_t)(exp + 1);
        }
    }
    if (temp > 1) d *= 2; /* remaining prime factor */
    return d;
}

seq_t a(idx_t n)
{
    /* Ensure primes up to n are available for pi(n). */
    if (n > primes_limit) {
        /* rebuild primes array to the new limit */
        free(primes);  /* free any existing allocation */
        build_primes_upto(n, &primes, &primes_cnt);
        primes_limit = n;
    }

    seq_t pi = (seq_t)primes_cnt;   /* number of primes <= n */
    seq_t d  = divisor_count(n);    /* tau(n) */

    return d * pi;
}
