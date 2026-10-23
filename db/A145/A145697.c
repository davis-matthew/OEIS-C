/*
 * A145697  Numbers n such that there exists x in N with (x+37)^3-x^3=n^2.
 * https://oeis.org/A145697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145697: a(n+2) = 590*a(n+1) - a(n),  a(1)=1369, a(2)=806341
   (Offset 1) */

#include <stdint.h>

/* Compute the nth term of the sequence.  The sequence is defined by a
   linear recurrence of order 2, so we can evaluate it iteratively in
   O(n) time with no extra memory.  This implementation works for
   any seq_t type supported by the build (uint8/16/32/64, __int128,
   or an arbitrary‑precision integer class compiled as C++17). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value outside the offset – undefined */
    if (n == 1) return (seq_t)1369;
    if (n == 2) return (seq_t)806341;

    seq_t a1 = (seq_t)1369;         /* a(1) */
    seq_t a2 = (seq_t)806341;       /* a(2) */
    seq_t an;                       /* current term */

    /* iterate from 3 up to the requested index n */
    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)590 * a2 - a1;  /* a(i) = 590*a(i-1) - a(i-2) */
        a1 = a2;                    /* shift previous terms */
        a2 = an;
    }

    return a2;
}
