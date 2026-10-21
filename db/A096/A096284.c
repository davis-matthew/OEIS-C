/*
 * A096284  Numerator of the ratio of the preceding two terms.
 * https://oeis.org/A096284
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = a(n-1) / a(n-2) with a(1)=1, a(2)=2; a(n) returns the
 *            numerator of the reduced fraction a(n). The sequence of
 *            numerators is periodic with period 6: 2,1,1,1,1,2 for n ≥ 3
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values for n < 3 are outside the OEIS offset; return 0 in that case */
    if (n < 3) return 0;
    static const seq_t pat[6] = { (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2 };
    return pat[(n - 3) % 6];
}
