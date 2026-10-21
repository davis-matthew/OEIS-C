/*
 * A226017  Primes such that sum of odd digits is also prime.
 * https://oeis.org/A226017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_counts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function returns the n-th prime whose sum of odd decimal digits is also prime.
   It works for any seq_t and idx_t defined by the build, including arbitrary-precision
   seq_t where arithmetic operators are overloaded.
   The routine simply tests successive integers for primality and the odd-digit-sum
   property, counting qualifying primes until the desired index is reached.
*/

#include "../utils/is_prime_big.h"     /* is_prime_big(seq_t)   */
#include "../utils/digit_counts.h"    /* digit_counts(seq_t, unsigned char[10]) */

seq_t a(idx_t n)
{
    if (n == 0)                         /* undefined for n=0; return 0 */
        return (seq_t)0;

    idx_t found = 0;                    /* number of qualifying primes found */
    idx_t cand  = 2;                    /* current integer candidate          */

    while (1) {
        seq_t p = (seq_t)cand;          /* cast to seq_t for primality test   */

        if (is_prime_big(p)) {          /* check if p is prime                */
            unsigned char cnt[10] = {0};
            digit_counts(p, cnt);       /* count digits of p                  */

            seq_t sum = 0;
            for (int d = 1; d <= 9; d += 2) /* sum odd digits */
                sum += (seq_t)d * (seq_t)cnt[d];

            if (is_prime_big(sum)) {    /* sum must be prime as well          */
                ++found;
                if (found == n)         /* n-th qualifying prime found       */
                    return p;
            }
        }

        /* next candidate: 3 after 2, then only odd numbers */
        cand += (cand == 2) ? 1 : 2;
    }
}
