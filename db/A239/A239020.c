/*
 * A239020  Smallest number k such that k*n +/- 1 and k*n^2 +/- 1 are two sets
 *          of twin primes. a(n) = 0 if no such number exists.
 * https://oeis.org/A239020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239020
   Formula: a(n) = smallest k such that k*n±1 and k*n^2±1 are both twin primes;
   a(n) = 0 if no such k exists.
   Implementation uses the provided is_prime_big function for primality testing.
*/

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 1.  From the OEIS table a(1) = 4. */
    if (n == 1)
        return 4;

    /* Determine gcd(n,6). Only four possibilities. */
    idx_t d = (n % 6 == 0) ? 6 :
              (n % 3 == 0) ? 3 :
              (n % 2 == 0) ? 2 : 1;

    /* Step size for k: k must be a multiple of 6/gcd(n,6). */
    seq_t step = 6 / d;               /* step ∈ {1,2,3,6} */

    /* Search for the smallest k. */
    for (seq_t k = step; ; k += step) {
        /* Center of the first twin prime set */
        seq_t t = k * (seq_t)n;
        seq_t t_minus = t - 1;
        seq_t t_plus  = t + 1;

        /* Check if t-1 and t+1 are prime. */
        if (!is_prime_big(t_minus) || !is_prime_big(t_plus))
            continue;

        /* Center of the second twin prime set */
        seq_t y = t * (seq_t)n;      /* k*n^2 = (k*n)*n */
        seq_t y_minus = y - 1;
        seq_t y_plus  = y + 1;

        if (is_prime_big(y_minus) && is_prime_big(y_plus))
            return k;                /* Found the smallest k */
    }
}
