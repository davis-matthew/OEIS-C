/*
 * A105683  Weight distribution of [12,6,6]_3 ternary extended Golay code.
 * https://oeis.org/A105683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A105683: Weight distribution of the [12,6,6]_3 ternary extended
   Golay code.  For n≥0,
   a(0) = 1
   a(1) = 0
   a(2) = 264   (weight 6)
   a(3) = 440   (weight 9)
   a(4) = 24    (weight 12)
   a(n) = 0 for all other n.
   (Weights are multiples of 3: a(n)=number of codewords of weight 3n.)
*/
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 2u) return (seq_t)264;
    if (n == 3u) return (seq_t)440;
    if (n == 4u) return (seq_t)24;
    return (seq_t)0;
}
