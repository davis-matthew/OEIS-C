/*
 * A182568  a(n) = 2*floor(n/4)*(n - 2*(1 + floor(n/4))).
 * https://oeis.org/A182568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/4)*(n - 2*(1 + floor(n/4)))
 * Range:     u8 n <= 33, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fn = (seq_t)n;          /* n as seq_t */
    seq_t q  = fn / 4;            /* floor(n/4) */
    return 2 * q * (fn - 2 * (1 + q));
}
