/*
 * A121755  Numerator of Sum/Product of first n primes = Numerator[ A007504[n]
 *          / A002110[n] ].
 * https://oeis.org/A121755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = Numerator[ Sum_{k=1..n} p_k / Product_{k=1..n} p_k ]
 * Range:     u8 n <= 13, u16 n <= 157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the numerator of the reduced fraction (sum of first n primes)
/   divided by the product of the first n primes.  */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *primes = NULL;   /* cache of the first primes */
static idx_t   prime_num = 0;     /* number of primes cached */

/* Ensure we have at least n primes in the cache */
static void
ensure_primes(idx_t n)
{
    if (prime_num < n) {
        uint32_t *new_primes;
        size_t new_cnt;
        sieve_first_n_primes(n, &new_primes, &new_cnt);
        /* Replace old cache; we avoid freeing because the array is stored in static memory. */
        if (primes)
            free(primes);
        primes    = new_primes;
        prime_num = (idx_t)new_cnt;   /* should equal n */
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n < 1, but safe guard */

    ensure_primes(n);

    /* Sum of first n primes */
    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i)
        sum += (seq_t)primes[i];

    /* Compute product of primes that divide the sum */
    seq_t g = 1;
    for (idx_t i = 0; i < n; ++i) {
        seq_t p = (seq_t)primes[i];
        if ((sum % p) == 0)
            g *= p;
    }

    /* Return numerator of reduced fraction */
    return sum / g;
}
