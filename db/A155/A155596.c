/*
 * A155596  a(n) = 5^n - 2^n + 1^n.
 * https://oeis.org/A155596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 2^n + 1^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow5 *= (seq_t)5;

    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= (seq_t)2;

    return pow5 - pow2 + (seq_t)1;
}
