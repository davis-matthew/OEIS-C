/*
 * A070658  a(n) = n^6 mod 35.
 * https://oeis.org/A070658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 35
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)(n % 35);
    seq_t result = 1;
    for (int i = 0; i < 6; ++i)
        result = (result * base) % 35;
    return result;
}
