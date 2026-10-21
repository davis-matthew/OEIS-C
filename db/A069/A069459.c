/*
 * A069459  a(n) = prime(n)^n - 1.
 * https://oeis.org/A069459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^n - 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   seq_pow, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* a(1) is defined; for n == 0 return 0 (undefined case) */
    if (n == 0)
        return 0;

    /* Allocate the first n primes: primes[0] = 2, primes[1] = 3, ... */
    uint32_t *primes = NULL;
    size_t count = 0;
    sieve_first_n_primes(n, &primes, &count);

    /* If the sieve failed, return 0 as a safe fallback. */
    if (count < (size_t)n) {
        if (primes)
            free(primes);
        return 0;
    }

    seq_t base   = (seq_t)primes[n - 1]; /* nth prime, as seq_t */
    seq_t value  = seq_pow(base, n);     /* compute prime^n */
    value = value - 1;                   /* subtract 1 per definition */

    free(primes);
    return value;
}
