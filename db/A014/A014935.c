/*
 * A014935  a(1)=1, a(n) = n*18^(n-1) + a(n-1).
 * https://oeis.org/A014935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*a(n-1) - 324*a(n-2) + 1 for n > 2 with a(1) = 1, a(2)
 *            = 37. This implements OEIS A014935
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) is the first term; undefined (or 0) for n<1. */
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)37;

    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)37;  /* a(2) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = ((seq_t)36 * a2) - ((seq_t)324 * a1) + (seq_t)1;
        a1 = a2;
        a2 = an;
    }

    return a2;
}
