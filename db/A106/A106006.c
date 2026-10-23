/*
 * A106006  a(n) = [n/2] + [n/3] + [n/5].
 * https://oeis.org/A106006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) + floor(n/3) + floor(n/5)
 * Range:     u8 n <= 248, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t2 = (seq_t)(n / 2);
    seq_t t3 = (seq_t)(n / 3);
    seq_t t5 = (seq_t)(n / 5);
    return t2 + t3 + t5;
}
