/*
 * A219284  Primes of the form 17^n - 16^n.
 * https://oeis.org/A219284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref
 * Formula:   a(n) = the n-th prime of the form 17^k - 16^k (in increasing
 *            order)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 2, big any n
 * Helpers:   grow_seq, is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"
#include "../utils/mul_small.h"

/* Static state for memoisation ------------------------------------------------ */
static seq_t *primes = NULL;   /* array of found primes  */
static size_t  primes_cap = 0; /* allocated capacity     */
static size_t  primes_len = 0; /* number of primes stored*/
static idx_t   next_exp = 0;    /* exponent for next check */
static seq_t   pow16 = 1, pow17 = 1; /* 16^e and 17^e for next_exp */

/* ----------------------------------------------------------------------------- */
/* Ensure that at least n primes have been found.                                */
static void ensure_terms(idx_t n)
{
    /* initialise array if necessary */
    if (primes == NULL) {
        grow_seq(&primes, &primes_cap, 16);
        primes_len = 0;
    }

    /* iterate exponents until we have enough primes */
    while (primes_len < (size_t)n) {
        /* advance exponent and update powers of 16 and 17 */
        next_exp++;
        pow16 = mul_small(pow16, 16);
        pow17 = mul_small(pow17, 17);

        /* compute difference 17^k - 16^k */
        seq_t diff = pow17;
        diff -= pow16;

        /* test primality */
        if (is_prime_big(diff)) {
            /* grow storage if needed */
            grow_seq(&primes, &primes_cap, primes_len + 1);
            primes[primes_len++] = diff;
        }
    }
}

/* ----------------------------------------------------------------------------- */
/* Required exported function                                                     */
seq_t a(idx_t n)
{
    if (n == 0)          /* no element for a(0) by convention */
        return 0;
    ensure_terms(n);
    return primes[(size_t)n - 1];
}
