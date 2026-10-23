/*
 * A198967  a(n) = 10*9^n-1.
 * https://oeis.org/A198967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*9^n-1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Returns the n-th term of OEIS A198967.
   a(0) = 9, a(1) = 89, a(2) = 809, …  */
seq_t a(idx_t n)
{
    /* Binary exponentiation of 9^n in type seq_t. */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)9;
    idx_t exp    = n;

    while (exp != 0)
    {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }

    /* Multiply by 10 and subtract 1. */
    result = result * (seq_t)10;
    result = result - (seq_t)1;
    return result;
}
