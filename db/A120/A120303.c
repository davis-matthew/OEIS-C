/*
 * A120303  Largest prime factor of Catalan number A000108(n).
 * https://oeis.org/A120303
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2, if n == 2; otherwise the largest prime < 2*n
 * Range:     u8 n <= 128, u16 n <= 800, u32 n <= 800, u64 n <= 800,
 *            u128 n <= 800, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the OEIS comment that for n>2 the largest prime less than 2*n
   equals the largest prime factor of the Catalan number C_n. */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)2;

    idx_t limit = 2 * n;

    /* Search downward from limit-1 until a prime is found. */
    for (idx_t p = limit - 1; p > 1; --p)
    {
        if (is_prime(p))
            return (seq_t)p;
    }

    /* In theory this line is unreachable because 2 is always prime. */
    return (seq_t)2;
}
