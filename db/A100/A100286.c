/*
 * A100286  Expansion of (1+2*x^2-2*x^3+2*x^4)/(1-x+x^2-x^3+x^4-x^5).
 * https://oeis.org/A100286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-6); a(0)=1, a(1)=1, a(2)=2, a(3)=0, a(4)=0, a(5)=2
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic pattern of length 6 */
    static const seq_t tbl[6] = {1, 1, 2, 0, 0, 2};
    return tbl[n % 6];
}
