/*
 * A017136  a(n) = (8*n + 5)^12.
 * https://oeis.org/A017136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 5)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 4, u128 n <= 202,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 8 * n fits in seq_t because n is small (<= 10000). */
    seq_t val = (seq_t)8 * (seq_t)n + 5;
    seq_t t2 = val * val;         /* val^2      */
    seq_t t4 = t2 * t2;           /* val^4      */
    seq_t t8 = t4 * t4;           /* val^8      */
    seq_t result = t8 * t4;       /* val^8 * val^4 = val^12 */
    return result;
}
