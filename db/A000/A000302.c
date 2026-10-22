/*
 * A000302  Powers of 4: a(n) = 4^n.
 * https://oeis.org/A000302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice, core
 * Formula:   a(n) = 4^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base = 4;
    idx_t e = n;
    while (e > 0) {
        if (e & 1)
            result *= base;
        e >>= 1;
        if (e > 0)
            base *= base;
    }
    return result;
}
