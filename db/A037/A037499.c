/*
 * A037499  Base-6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2.
 * https://oeis.org/A037499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A037499: a(n) is the integer whose base-6 digits
   are the first n terms of the periodic sequence 1,0,2.  Recurrence:
   a(1)=1, a(n)=6*a(n-1)+digit((n-1) mod 3) where
   digit(0)=1, digit(1)=0, digit(2)=2.  (Offset: 1) */
seq_t a(idx_t n)
{
    static const unsigned char digits[3] = {1, 0, 2};
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
        res = res * 6 + (seq_t)digits[i % 3];
    return res;
}
