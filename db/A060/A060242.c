/*
 * A060242  a(n) = (2^n - 1)*(4^n - 1).
 * https://oeis.org/A060242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - 1)*(4^n - 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Compute 2^n and 4^n using the provided utility functions then
       form the value (2^n - 1) * (4^n - 1).  The same implementation
       works for exact integer, 128‑bit, or arbitrary‑precision builds. */
    seq_t p2 = pow2(n);
    seq_t p4 = pow4(n);
    return (p2 - 1) * (p4 - 1);
}
