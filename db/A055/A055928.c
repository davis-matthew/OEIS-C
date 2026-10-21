/*
 * A055928  Sum of square divisors of n! = sum of squares of divisors of the
 *          square root of largest square dividing n!.
 * https://oeis.org/A055928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p ≤ n} ( 1 + p² + p⁴ + … + p^{2·⌊e_p/2⌋} ) where e_p
 *            is the exponent of prime p in n!
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 39,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes.h"   /* build_primes */

static idx_t max_limit = 0;          /* current upper bound for primes cached */
static idx_t *prime_list = NULL;     /* array of primes ≤ max_limit */
static size_t prime_count = 0;       /* number of primes in prime_list */

/* Compute a(n) as defined above. */
seq_t a(idx_t n)
{
    /* Cache primes up to n if not already done. */
    if (n > max_limit) {
        if (prime_list) free(prime_list);
        build_primes(n, (idx_t **)&prime_list, &prime_count);
        max_limit = n;
    }

    seq_t result = 1;  /* running product of the series for each prime */

    for (size_t idx = 0; idx < prime_count; ++idx) {
        idx_t p = prime_list[idx];
        if (p > n) break;

        /* exponent of p in n! */
        idx_t e = 0;
        for (idx_t power = p; power <= n; power *= p)
            e += n / power;

        idx_t sq_e = e / 2;          /* floor(e/2) */
        if (sq_e == 0) continue;     /* series equals 1, skip multiplication */

        /* Compute geometric series sum: 1 + p² + p⁴ + … + p^{2·sq_e} */
        seq_t pp   = (seq_t)p * (seq_t)p;   /* p² */
        seq_t term = 1;                     /* current power of pp (p^{2k}) */
        seq_t sum  = 1;                     /* sum of powers */

        for (idx_t i = 1; i <= sq_e; ++i) {
            term = term * pp;               /* term *= p² */
            sum  = sum + term;             /* sum += term */
        }

        result = result * sum;              /* multiply into total product */
    }

    return result;
}
