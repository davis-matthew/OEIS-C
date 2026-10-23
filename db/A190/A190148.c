/*
 * A190148  Largest prime factor of the least number having exactly two odd
 *          prime factors that differ by 2*n^2.
 * https://oeis.org/A190148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the larger prime of the least product p*(p+2*n^2) where p
 *            and p+2*n^2 are odd primes (the smallest such p)
 * Range:     u8 n <= 10, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n=0 */
    seq_t diff = (seq_t)2 * n * n;             /* diff = 2*n^2 in seq_t */

    for (seq_t p = 3;; p += 2)                 /* iterate odd primes candidates */
    {
        if (is_prime_big(p) && is_prime_big(p + diff))
            return p + diff;                   /* largest prime factor of the pair */
    }
}
