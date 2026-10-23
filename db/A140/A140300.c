/*
 * A140300  a(n) = 1024^n.
 * https://oeis.org/A140300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1024^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)1024;
    while (n) {
        if (n & 1U) {
            result *= base;
        }
        base *= base;
        n >>= 1U;
    }
    return result;
}
