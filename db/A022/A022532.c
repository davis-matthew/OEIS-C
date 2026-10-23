/*
 * A022532  Nexus numbers (n+1)^16-n^16.
 * https://oeis.org/A022532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)^16 - n^16
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 15, u128 n <= 307,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t xn   = (seq_t)n;
    seq_t xn1  = xn + 1;
    seq_t pow_n   = xn;
    seq_t pow_n1  = xn1;

    /* n^16 and (n+1)^16 via repeated squaring */
    pow_n   *= pow_n;   /* n^2  */
    pow_n   *= pow_n;   /* n^4  */
    pow_n   *= pow_n;   /* n^8  */
    pow_n   *= pow_n;   /* n^16 */

    pow_n1  *= pow_n1;  /* (n+1)^2  */
    pow_n1  *= pow_n1;  /* (n+1)^4  */
    pow_n1  *= pow_n1;  /* (n+1)^8  */
    pow_n1  *= pow_n1;  /* (n+1)^16 */

    return pow_n1 - pow_n;
}
