/*
 * A117819  Final digit d of n is in the sequence iff d is prime or n is
 *          prime.
 * https://oeis.org/A117819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = last digit of the n-th integer m ≥ 2 such that * m is
 *            prime or its last digit is one of {2,3,5,7}
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Note: the sequence offsets by 1, so a(1) corresponds to the first
 *  qualifying integer m (which is 2). */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1, but return 0 for convenience */
        return (seq_t)0;

    idx_t count = 0;
    idx_t m = 2;          /* start checking from 2 */

    while (1) {
        /* Check the condition: prime or last digit is a prime digit */
        if (is_prime(m) ||
            (m % 10u) == 2u ||
            (m % 10u) == 3u ||
            (m % 10u) == 5u ||
            (m % 10u) == 7u)
        {
            count++;
            if (count == n) {
                /* Return the last digit as seq_t */
                return (seq_t)(m % 10u);
            }
        }
        m++;
    }
}
