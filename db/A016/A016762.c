/*
 * A016762  a(n) = (2*n + 1)^10.
 * https://oeis.org/A016762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)^10
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 41, u128 n <= 3565,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 2 + 1;
    seq_t result = 1;
    for (int i = 0; i < 10; ++i) {
        result *= base;
    }
    return result;
}
