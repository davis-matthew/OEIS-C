/*
 * A016936  a(n) = (6*n + 2)^4.
 * https://oeis.org/A016936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 2)^4
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 42, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)6 * n + 2;
    seq_t sq   = base * base;
    return sq * sq;
}
