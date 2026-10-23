/*
 * A013717  a(n) = 12^(2*n + 1).
 * https://oeis.org/A013717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^(2*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponent e = 2*n + 1, computed in idx_t to avoid overflow */
    idx_t e = 2*n + 1;
    seq_t result = 1;
    seq_t base = 12;
    while (e > 0) {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}
