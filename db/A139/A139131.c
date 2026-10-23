/*
 * A139131  Squarefree kernel of n*(n+1)/2.
 * https://oeis.org/A139131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = squarefree kernel of n*(n+1)/2 = rad(n*(n+1)/2)
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute T_n = n*(n+1)/2 first, using seq_t arithmetic. */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    seq_t rad = 1;           /* product of distinct prime factors */
    seq_t temp = tri;

    /* Simple trial division.  For n up to 10 000 this is quick enough. */
    for (idx_t p = 2; (seq_t)p * (seq_t)p <= temp; ++p) {
        if (temp % p == 0) {
            rad *= p;               /* include prime once */
            while (temp % p == 0)   /* strip all copies */
                temp /= p;
        }
    }

    /* If something remains, it is a prime larger than sqrt(original tri). */
    if (temp > 1)
        rad *= temp;

    return rad;
}
