/*
 * A037675  Base-5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2,3.
 * https://oeis.org/A037675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Base-5 digits are, in order, the first n terms of the
   periodic sequence 1,0,2,3.  Thus a(n) = a(n-1)*5 + d_{n-1},
   where d_k = {1,0,2,3}[k % 4] and a(0)=0 (a(1)=1). */

seq_t a(idx_t n)
{
    /* Return 0 for n==0 (out of offset, value unused). */
    if (n == 0) return 0;

    static const unsigned int period[4] = {1, 0, 2, 3};

    seq_t value = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        value = value * (seq_t)5 + (seq_t)period[i & 3];
    }
    return value;
}
