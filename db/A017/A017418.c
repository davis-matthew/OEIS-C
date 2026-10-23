/*
 * A017418  a(n) = (11*n + 2)^6.
 * https://oeis.org/A017418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 2)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 147, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base  = (seq_t)11 * n + (seq_t)2;
    seq_t base2 = base * base;        /* base^2   */
    seq_t base3 = base2 * base;       /* base^3   */
    seq_t result = base3 * base3;     /* (base^3)^2 = base^6 */
    return result;
}
