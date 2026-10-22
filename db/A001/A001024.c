/*
 * A001024  Powers of 15: a(n) = 15^n.
 * https://oeis.org/A001024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base = 15;
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
