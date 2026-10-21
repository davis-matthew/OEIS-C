/*
 * A070593  a(n) = n^5 mod 7.
 * https://oeis.org/A070593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 7
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nm = (seq_t)(n % 7);
    seq_t result = 1;
    for (int i = 0; i < 5; ++i)
        result = (result * nm) % 7;
    return result;
}
