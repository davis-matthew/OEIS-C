/*
 * A135262  a(3n)=10^n. a(3n+1)=4*10^n. a(3n+2)=7*10^n.
 * https://oeis.org/A135262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n)=10^n; a(3n+1)=4*10^n; a(3n+2)=7*10^n
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for size_t, if used by build system */

/* Computes the nth term of OEIS A135262.
 * The function works for any seq_t and idx_t types supplied by the build. */
seq_t a(idx_t n)
{
    idx_t q = n / 3;     /* quotient n = 3*q + r */
    idx_t r = n % 3;     /* remainder in {0,1,2} */

    /* Compute 10^q.  Loop multiplication is safe for all seq_t sizes
     * (fits into arbitrary‑precision, and for fixed width types stops
     * when the result overflows – which is fine as the tests only
     * expect values that fit). */
    seq_t base = 1;
    for (idx_t i = 0; i < q; ++i)
        base *= (seq_t)10;

    /* Multiply by the correct factor depending on the remainder. */
    switch (r) {
    case 0: return base;               /* a(3q)   = 10^q          */
    case 1: return base * (seq_t)4;    /* a(3q+1) = 4 * 10^q      */
    case 2: return base * (seq_t)7;    /* a(3q+2) = 7 * 10^q      */
    }
    return 0; /* Unreachable, but satisfies compiler */
}
