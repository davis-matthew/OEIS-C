/*
 * A185276  a(n) = Kronecker symbol (-100 / n).
 * https://oeis.org/A185276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Kronecker(-100 / n)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0 */
/* For n > 0:  a(n) = 0 if n is even or divisible by 5; */
/* otherwise a(n) = (-1 / n) = (-1)^((n-1)/2) = 1 when n ≡ 1 (mod 4), */
/* and -1 when n ≡ 3 (mod 4). */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Zero for multiples of 2 or 5 */
    if ((n & 1ULL) == 0 || (n % 5ULL) == 0)
        return 0;

    /* For odd n not divisible by 5, the value is (-1/n) */
    if ((n % 4ULL) == 1ULL)
        return (seq_t)1;
    else
        return (seq_t)-1;
}
