/*
 * A070627  a(n) = n^5 mod 44.
 * https://oeis.org/A070627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 44
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n modulo 44 first to keep intermediate values small */
    seq_t m   = (seq_t)n % 44;   /* m = n mod 44 */
    seq_t m2  = (m * m) % 44;    /* m^2 mod 44 */
    seq_t m4  = (m2 * m2) % 44;  /* m^4 mod 44 */
    seq_t m5  = (m4 * m) % 44;   /* m^5 mod 44 */
    return m5;
}
