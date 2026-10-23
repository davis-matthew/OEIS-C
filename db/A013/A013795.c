/*
 * A013795  a(n) = 11^(4*n+3).
 * https://oeis.org/A013795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^(4*n+3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t exp = 4 * n + 3;
    seq_t result = 1;
    seq_t base = 11;
    while (exp != 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
