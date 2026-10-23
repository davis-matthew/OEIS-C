/*
 * A037236  Expansion of (3+2*x^2)/(1-x)^4.
 * https://oeis.org/A037236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(5*n^2+13*n+18)/6
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* n as seq_t */
    seq_t x2 = x * x;            /* n^2 */
    seq_t term = x2 * 5;         /* 5*n^2 */
    term = term + x * 13;        /* +13*n */
    term = term + 18;            /* +18 */
    seq_t result = (x + 1) * term; /* (n+1)*(...) */
    result = result / 6;          /* divide by 6 */
    return result;
}
