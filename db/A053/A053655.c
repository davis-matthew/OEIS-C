/*
 * A053655  a(n) = (10^n - 1)*(10^(2*n-1) - 1)/81.
 * https://oeis.org/A053655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n - 1)*(10^(2*n-1) - 1)/81
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   repunit_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/repunit_of.h"
/*  = repunit(n) * repunit(2*n-1)  for n >= 1  */

seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is 1‑indexed; a(0) is unused */
        return 0;

    seq_t first  = repunit_of(n);       /* (10^n − 1)/9  */
    seq_t second = repunit_of(2*n-1);   /* (10^(2n‑1) − 1)/9 */

    /* For 64‑bit seq_t the product never overflows for the values
       that fit in 64 bits (a(n) that are tested).  For larger
       seq_t types (128‑bit or arbitrary‑precision) the multiplication
       is handled by that type.  No extra casts are needed.       */
    return first * second;
}
