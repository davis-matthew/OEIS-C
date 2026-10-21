/*
 * A089121  Duplicate of A014442.
 * https://oeis.org/A089121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = largest prime factor of n^2 + 1
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 22, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute m = n^2 + 1, using seq_t */
    seq_t m = (seq_t)n * (seq_t)n + (seq_t)1;

    seq_t temp = m;
    seq_t largest = (seq_t)0;

    /* factor out 2 */
    if ((temp & (seq_t)1) == (seq_t)0) {
        largest = (seq_t)2;
        while ((temp & (seq_t)1) == (seq_t)0)
            temp >>= 1; /* temp /= 2 */
    }

    /* factor odd divisors */
    seq_t i = (seq_t)3;
    while (i <= temp / i) {
        if (temp % i == (seq_t)0) {
            largest = i;
            while (temp % i == (seq_t)0)
                temp /= i;
        }
        i += (seq_t)2;
    }

    /* if remaining factor > 1 it is a prime */
    if (temp > (seq_t)1 && temp > largest)
        largest = temp;

    /* for n=1, m=2, largest already set to 2 */
    return largest;
}
