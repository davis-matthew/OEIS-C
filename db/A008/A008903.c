/*
 * A008903  x->x/2 if x even, x->3x-1 if x odd.
 * https://oeis.org/A008903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = repeated application of x -> (x%2==0?x/2:3*x-1) starting
 *            from a(0) = 100
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes the nth term of OEIS sequence A008903. */
seq_t a(idx_t n)
{
    seq_t x = 100;                      /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        if ((x & 1) == 0)               /* even */
            x = x / 2;
        else                            /* odd */
            x = 3 * x - 1;
    }
    return x;
}
