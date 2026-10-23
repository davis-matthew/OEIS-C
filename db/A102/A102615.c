/*
 * A102615  Nonprime numbers of order 2.
 * https://oeis.org/A102615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nonprime(nonprime(n))
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

/* Return the n-th nonprime number (1 and composites). */
static inline seq_t nth_nonprime(idx_t n)
{
    idx_t count = 0;
    idx_t x = 1;
    while (1)
    {
        if (x == 1 || !is_prime(x))
        {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
        ++x;
    }
}

seq_t a(idx_t n)
{
    seq_t first = nth_nonprime(n);
    return nth_nonprime((idx_t)first);
}
