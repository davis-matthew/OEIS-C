/*
 * A023022  Number of partitions of n into two relatively prime parts. After
 *          initial term, this is the "half-totient" function phi(n)/2
 *          (A000010(n)/2).
 * https://oeis.org/A023022
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n)/2 for n >= 3; a(2) = 1 (the offset starts at n =
 *            2)
 * Range:     u8 n <= 398, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute Euler's totient of n (n >= 1).  Returns a value that fits in seq_t. */
static seq_t __phi_u64(idx_t n)
{
    seq_t result = (seq_t)n;
    idx_t tmp    = n;

    idx_t i = 2;
    while ((idx_t)i * (idx_t)i <= tmp)
    {
        if (tmp % i == 0)
        {
            while (tmp % i == 0)
                tmp /= i;
            result -= result / (seq_t)i;
        }
        i++;
    }
    if (tmp > 1)
        result -= result / (seq_t)tmp;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* value for n < 2 is irrelevant (offset starts at 2) */

    if (n == 2)
        return 1;          /* a(2) is defined to be 1 */

    /* For n >= 3, a(n) = φ(n)/2.  φ(n) is even for n > 2. */
    return __phi_u64(n) / 2;
}
