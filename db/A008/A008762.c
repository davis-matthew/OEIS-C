/*
 * A008762  Expansion of (1+x)/((1-x)*(1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A008762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*n^3 + 27*n^2 + 108*n + 207)/144)
 * Range:     u8 n <= 0, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast n to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;             /* n^2 */
    seq_t n3 = n2 * nn;             /* n^3 */
    seq_t num = (seq_t)2 * n3 + (seq_t)27 * n2 + (seq_t)108 * nn + (seq_t)207;
    return num / (seq_t)144;
}
