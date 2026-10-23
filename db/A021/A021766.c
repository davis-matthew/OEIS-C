/*
 * A021766  Decimal expansion of 1/762.
 * https://oeis.org/A021766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 762) % 10 (digit n of the decimal
 *            expansion of 1/762, offset 0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* r = 10^n mod 762;  n can be large but we accumulate
       it with O(n) iterations, which is fast for n ≤ 10⁴.  */
    idx_t r = 1;                 /* r = 10^n mod 762 */
    for (idx_t i = 0; i < n; ++i) {
        r = (r * 10) % 762;
    }
    /* digit = floor((r*10) / 762)   (0 ≤ digit ≤ 9) */
    return (seq_t)((r * 10) / 762);
}
