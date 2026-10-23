/*
 * A189102  Greatest common divisor of n! and its arithmetic derivative.
 * https://oeis.org/A189102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n!, (n!)') = product over primes p≤n of p^{v_p(n!) -
 *            δ_p}, where δ_p = 1 if p ∤ v_p(n!), else 0
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 44,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the helper build_primes_upto to obtain the prime
   list up to n, computes v_p(n!) for each prime, tests whether p divides
   v_p(n!), and finally multiplies the appropriate power of p into the
   result using seq_pow and seq_mul. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/seq_pow.h"             /* seq_pow */
#include "../utils/seq_mul.h"             /* seq_mul */

seq_t a(idx_t n)
{
    /* Static cache of primes */
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 0;

    /* Grow prime list if needed */
    if (n > prime_limit) {
        /* Free old list */
        free(primes);
        primes = NULL;
        /* Build new list of primes up to n */
        build_primes_upto(n, &primes, &prime_count);
        prime_limit = n;
    }

    seq_t result = seq_pow((seq_t)1, (idx_t)0); /* start with 1 */

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];

        /* Compute v_p(n!) */
        idx_t t = n;
        idx_t vp = 0;
        while (t) {
            t /= p;
            vp += t;
        }

        /* Determine if p divides vp (integer division) */
        idx_t exp = vp;
        if (vp % p != 0) {
            /* Remove one factor of p from the exponent */
            exp = vp - 1;
        }

        if (exp == 0)
            continue;   /* multiplying by 1 */

        seq_t ppow = seq_pow((seq_t)p, exp);
        result = seq_mul(result, ppow);
    }

    return result;
}
