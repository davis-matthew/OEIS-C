/*
 * A106058  4th diagonal of triangle in A059317.
 * https://oeis.org/A106058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n<=2; a(n) = (n-2)*(n^2 + 8n - 21)/6 for n>2
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t t  = nn * nn + 8 * nn - 21;     /* n^2 + 8n - 21 */
    seq_t num = (seq_t)(n - 2) * t;        /* (n-2)*(...) */
    return num / 6;
}
