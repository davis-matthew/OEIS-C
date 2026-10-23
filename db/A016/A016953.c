/*
 * A016953  a(n) = (6*n + 3)^9.
 * https://oeis.org/A016953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 3)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 22, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = 6*n + 3  (cast to seq_t to avoid intermediate overflows) */
    seq_t base = ((seq_t)6 * (seq_t)n) + (seq_t)3;

    /* compute powers by repeated multiplication:
       b2  = base^2
       b3  = base^3
       b6  = base^6
       result = base^9 = b6 * b3
    */
    seq_t b2 = base * base;
    seq_t b3 = b2 * base;
    seq_t b6 = b3 * b3;
    seq_t result = b6 * b3;

    return result;
}
