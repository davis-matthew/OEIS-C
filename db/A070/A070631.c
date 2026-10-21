/*
 * A070631  a(n) = n^5 mod 48.
 * https://oeis.org/A070631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 48
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n % 48;
    unsigned long long prod = 1;
    for (int i = 0; i < 5; ++i)
        prod = (prod * (unsigned long long)sn) % 48;
    return (seq_t)prod;
}
