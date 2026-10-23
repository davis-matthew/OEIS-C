/*
 * A165808  Expansion of x*(403+2967*x+1047*x^2-x^3)/(1-x)^4.
 * https://oeis.org/A165808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 736*n^3 - 321*n^2 - 13*n + 1
 * Range:     u8 none, u16 n <= 4, u32 n <= 180, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* sequence index as seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */

    seq_t result = 736 * n3;      /* 736*n^3 */
    result -= 321 * n2;           /* -321*n^2 */
    result -= 13 * nn;            /* -13*n   */
    result += 1;                  /* +1      */

    return result;
}
