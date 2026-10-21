/*
 * A060762  Number of conjugacy classes (the same as the number of irreducible
 *          representations) in the dihedral group with 2n elements.
 * https://oeis.org/A060762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)/2 if n is odd, (n+6)/2 if n is even
 * Range:     u8 n <= 505, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n/2 without overflow. */
    seq_t half = (seq_t)(n >> 1);
    if (n & 1)   /* n is odd */
        return half + 2;  /* (n+3)/2 = n/2 + 2  */
    else         /* n is even */
        return half + 3;  /* (n+6)/2 = n/2 + 3  */
}
