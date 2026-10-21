/*
 * A099396  a(n) = floor(log_2((2/3)*n)) for n >= 2, a(1) = 0.
 * https://oeis.org/A099396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log2((2/3)*n)) for n >= 2, a(1) = 0
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    /* Compute floor((2/3)*n) using integer arithmetic */
    seq_t x = ((seq_t)n * 2) / 3;
    if (x == 0)   /* for safety, though x should be >= 1 when n>=2 */
        return 0;

    /* floor(log2(x)) = bitlen(x) - 1, bitlen_seq_t returns
       the number of bits needed to represent x (i.e., floor(log2(x)) + 1) */
    return (seq_t)(bitlen_seq_t(x) - 1);
}
