/*
 * A043547  Odd numbers interspersed with double the previous odd number.
 * https://oeis.org/A043547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n-1)=2n-1, a(2n)=4n-2
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n ≥ 1, the value is the index itself if n is odd,
       otherwise it is twice the preceding odd number: 2*(n-1). */
    if (n & 1U)                     /* odd index -> odd value */
        return (seq_t)n;
    else                            /* even index -> double the previous odd */
        return ((seq_t)(n - 1U)) << 1;
}
