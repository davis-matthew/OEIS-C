/*
 * A104037  Numbers of primes between two sexy primes.
 * https://oeis.org/A104037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A104037: Number of primes between the n-th sexy prime pair (p,p+6).
 * Implementation: iterate over integers, test primality using is_prime,
 * count sexy prime pairs, and when the n-th pair is found, count how many
 * primes lie strictly between p and p+6.
 */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1; defensive */
    idx_t          count_pairs = 0;        /* number of sexy pairs seen so far */
    idx_t          p = 5;                  /* smallest possible first prime of a pair */

    while (1)
    {
        /* check if p and p+6 are both prime */
        if (is_prime(p) && is_prime(p + 6))
        {
            count_pairs++;
            if (count_pairs == n)           /* n-th sexy pair found */
            {
                seq_t primes_between = 0;
                if (is_prime(p + 1)) primes_between++;
                if (is_prime(p + 2)) primes_between++;
                if (is_prime(p + 3)) primes_between++;
                if (is_prime(p + 4)) primes_between++;
                if (is_prime(p + 5)) primes_between++;
                return primes_between;
            }
        }
        p++;                                 /* next candidate for first prime */
    }
}
