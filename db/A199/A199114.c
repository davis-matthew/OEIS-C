/*
 * A199114  a(n) = 11*3^n+1.
 * https://oeis.org/A199114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*3^n+1
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 3;
    idx_t e = n;
    while (e) {
        if (e & 1)
            result *= base;
        e >>= 1;
        if (e)
            base *= base;
    }
    result *= 11;
    result += 1;
    return result;
}
