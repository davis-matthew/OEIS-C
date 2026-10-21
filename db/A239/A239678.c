/*
 * A239678  Least numbers k such that k*2^n+1 is a square.
 * https://oeis.org/A239678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239678:
   a(0) = 3, a(1) = 4, a(2) = 2;
   for n >= 3: a(n) = 2^(n-2) - 1
   Since a(n) is a simple power of two minus one, we use pow2() from
   the utilities to handle arbitrary‑precision seq_t types. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)3;
    if (n == (idx_t)1) return (seq_t)4;
    if (n == (idx_t)2) return (seq_t)2;
    seq_t v = pow2(n - 2);   /* 2^(n-2) as seq_t */
    return v - (seq_t)1;     /* subtract 1 */
}
