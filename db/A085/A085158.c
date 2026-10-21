/*
 * A085158  Sextuple factorials, 6-factorials, n!!!!!!, n!6.
 * https://oeis.org/A085158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 43, u64 n <= 75, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085158 – Sextuple factorials
   Recurrence: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5
               a(n) = n * a(n-6)   for n ≥ 6
   This implementation computes the product n·(n-6)·(n-12)····
   down to the largest index < 6, then multiplies by the base
   value for that remainder.  The operation is performed in
   seq_t, preserving full precision for arbitrary‑precision
   builds and remaining accurate for any value that fits in
   the selected seq_t size. */
#include <stdint.h>

static const idx_t _aea_base[6] = {1, 1, 2, 3, 4, 5};

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;          /* start with multiplicative identity */
    idx_t i = n;

    /* Multiply n, n-6, n-12, ... until < 6 */
    while (i >= 6)
    {
        result *= (seq_t)i;
        i -= 6;
    }

    /* Multiply by the appropriate base value for the remaining
       remainder (0‑5).  For n < 6, this simply returns the base
       value itself. */
    result *= (seq_t)_aea_base[i];
    return result;
}
