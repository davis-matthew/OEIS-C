/*
 * A117736  factorial(n) - A049614(n).
 * https://oeis.org/A117736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - n! / primorial(n) where primorial(n) = product of
 *            all primes ≤ n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* factorial */
    seq_t fact = factorial(n);

    /* primorial */
    seq_t prim = 1;
    if (n >= 2) {
        /* simple sieve up to n */
        uint8_t *is_prime = (uint8_t*)malloc((size_t)(n + 1));
        if (!is_prime) {
            /* fallback: prim stays 1, quotient will be fact/1 = fact, result = 0 */
            return 0;
        }
        for (idx_t i = 0; i <= n; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (idx_t p = 2; p * p <= n; ++p) {
            if (is_prime[p]) {
                for (idx_t m = p * p; m <= n; m += p)
                    is_prime[m] = 0;
            }
        }
        for (idx_t p = 2; p <= n; ++p) {
            if (is_prime[p])
                prim = prim * p;   /* multiply by small integer */
        }
        free(is_prime);
    }

    /* compute a(n) = fact - fact/prim */
    seq_t q = fact / prim;
    return fact - q;
}
