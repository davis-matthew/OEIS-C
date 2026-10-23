/*
 * A178915  Rearrangement of natural numbers so that every partial sum is
 *          composite.
 * https://oeis.org/A178915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n for n > 4, with a(1)=4 and a(4)=1 (the sequence is the
 *            natural numbers with 1 and 4 swapped)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)4;
    if (n == 4)
        return (seq_t)1;
    /* For all other n (including n=2,3 or n>4) the term equals n */
    return (seq_t)n;
}
