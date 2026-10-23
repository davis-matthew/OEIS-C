/*
 * A140282  Numbers k such that A000330(k) is multiple of 3.
 * https://oeis.org/A140282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + n mod 3
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast first to avoid overflows on smaller seq_t types */
    seq_t val = (seq_t)n;
    seq_t mod = (seq_t)(n % 3);
    return val * 3 + mod;
}
