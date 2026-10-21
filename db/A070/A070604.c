/*
 * A070604  n^5 mod 20.
 * https://oeis.org/A070604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 20
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)(n % 20);
    seq_t result = 1;
    for (int i = 0; i < 5; ++i)
        result = (result * base) % 20;
    return result;
}
