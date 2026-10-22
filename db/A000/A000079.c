/*
 * A000079  Powers of 2: a(n) = 2^n.
 * https://oeis.org/A000079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   a(n) = 2^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base = 2;
    while (n > 0) {
        if (n & 1)
            result *= base;
        n >>= 1;
        if (n > 0)
            base *= base;
    }
    return result;
}
