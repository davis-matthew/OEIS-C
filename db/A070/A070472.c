/*
 * A070472  a(n) = n^3 mod 7.
 * https://oeis.org/A070472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 7
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)(n % 7);
    seq_t r = m;
    r = (r * r) % 7;
    r = (r * m) % 7;
    return r;
}
