/*
 * A016860  a(n) = (5*n)^12.
 * https://oeis.org/A016860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n)^12
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 8, u128 n <= 325,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)5 * (seq_t)n;      /* (5*n) */
    seq_t b2   = base * base;             /* (5*n)^2 */
    seq_t b4   = b2 * b2;                 /* (5*n)^4 */
    seq_t b8   = b4 * b4;                 /* (5*n)^8 */
    seq_t b12  = b8 * b4;                 /* (5*n)^(8+4) = (5*n)^12 */
    return b12;
}
