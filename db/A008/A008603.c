/*
 * A008603  Multiples of 21.
 * https://oeis.org/A008603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*n
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th term of OEIS sequence A008603 (Multiples of 21),
   with offset 0: a(0)=0, a(1)=21, a(2)=42, etc. */
seq_t a(idx_t n)
{
    /* Multiply the index by 21.  The cast to seq_t ensures that
       the multiplication is done in the target type, which may be
       an unsigned integer or an arbitrary‑precision integer class. */
    return (seq_t)n * 21;
}
