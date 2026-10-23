/*
 * A013880  a(n) = 16^(5*n + 3).
 * https://oeis.org/A013880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^(5*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute exponent e = 20*n + 12  (since 16^(5*n+3) = 2^(20*n+12)) */
    idx_t e = (idx_t)n * 20 + 12;

    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;

    while (e > 0) {
        if (e & 1)
            result = result * base;
        base = base * base;
        e >>= 1;
    }
    return result;
}
