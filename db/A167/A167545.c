/*
 * A167545  n^6 mod 16.
 * https://oeis.org/A167545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6) mod 16
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n % 16;
    seq_t res = 1;
    for (int i = 0; i < 6; ++i)
        res = (res * x) % 16;
    return res;
}
