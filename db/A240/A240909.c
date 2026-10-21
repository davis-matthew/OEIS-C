/*
 * A240909  The sequency numbers of the 16 rows of a Hadamard-Walsh matrix of
 *          order 16.
 * https://oeis.org/A240909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A131271(4,n) - 1 The mapping for order 16 can be
 *            expressed in terms of the 4 least significant bits of (n-1): let
 *            b3 b2 b1 b0 be these bits (b3 is the most significant) then a3 =
 *            b0 a2 = b1 ^ b0 a1 = b2 ^ b1 ^ b0 a0 = b3 ^ b2 ^ b1 ^ b0 The
 *            result a(n) has bits a3 a2 a1 a0 (a3 is the most s...
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required header for integer types used by the build environment. */
#include <stddef.h>   /* for size_t */

/* Return the a(n) value for n >= 1.  The function is thread‑safe
   and has no side effects. */
seq_t a(idx_t n)
{
    /* Work with the 4 least significant bits of n-1. */
    idx_t i = n - 1;
    idx_t b0 = i & 1ULL;
    idx_t b1 = (i >> 1) & 1ULL;
    idx_t b2 = (i >> 2) & 1ULL;
    idx_t b3 = (i >> 3) & 1ULL;

    /* Compute the four bits of the output as described above. */
    seq_t a3 = (seq_t)b0;
    seq_t a2 = (seq_t)(b1 ^ b0);
    seq_t a1 = (seq_t)(b2 ^ b1 ^ b0);
    seq_t a0 = (seq_t)(b3 ^ b2 ^ b1 ^ b0);

    /* Assemble the final 4‑bit result. */
    return (a3 << 3) | (a2 << 2) | (a1 << 1) | a0;
}
