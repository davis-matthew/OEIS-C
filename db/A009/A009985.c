/*
 * A009985  Powers of 41.
 * https://oeis.org/A009985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 41^n ; a(0) = 1, a(n) = 41*a(n-1) for n>0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;   /* a(0) */
    seq_t base   = (seq_t)41;  /* base for exponentiation */

    /* binary exponentiation – works for any seq_t type */
    while (n != 0) {
        if (n & 1ULL)
            result *= base;
        base *= base;
        n >>= 1;
    }
    return result;
}
