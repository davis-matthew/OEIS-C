/*
 * A132027  a(n) = Product_{k=0..floor(log_3(n))} floor(n/3^k), n>=1.
 * https://oeis.org/A132027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0..⌊log_3 n⌋} ⌊n / 3^k⌋ Recurrence: a(n) = n *
 *            a(⌊n/3⌋), a(1)=1 (Implemented iteratively using the product
 *            definition.)
 * Range:     u8 n <= 20, u16 n <= 86, u32 n <= 674, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* idx_t: unsigned 64‑bit integer, seq_t: unsigned integer type (8,16,32,64,128 bits
   or arbitrary precision).  They are defined by the build system.  No need to
   redeclare them here. */

seq_t a(idx_t n)
{
    /* Return 0 for index 0, though the sequence is defined for n >= 1. */
    if (n == 0)
        return 0;

    seq_t result = 1;
    for (idx_t t = n; t > 0; t /= 3)
        result *= (seq_t)t;          /* multiply by ⌊n/3^k⌋ */

    return result;
}
