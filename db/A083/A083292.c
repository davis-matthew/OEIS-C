/*
 * A083292  a(n) = n*floor(n/10) + (n mod 10).
 * https://oeis.org/A083292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*floor(n/10) + (n mod 10)
 * Range:     u8 n <= 50, u16 n <= 809, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;           /* cast index to sequence type */
    seq_t div = sn / (seq_t)10;    /* floor(n/10) */
    seq_t rem = sn % (seq_t)10;    /* n mod 10 */
    return sn * div + rem;         /* a(n) = n * floor(n/10) + (n mod 10) */
}
