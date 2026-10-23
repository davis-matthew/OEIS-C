/*
 * A017658  a(n) = (12*n + 11)^6.
 * https://oeis.org/A017658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n+11)^6
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 134, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 12 + 11;
    seq_t result = base;
    result *= base; /* (12*n+11)^2 */
    result *= base; /* (12*n+11)^3 */
    result *= base; /* (12*n+11)^4 */
    result *= base; /* (12*n+11)^5 */
    result *= base; /* (12*n+11)^6 */
    return result;
}
