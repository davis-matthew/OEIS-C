/*
 * A013724  a(n) = 19^(2*n + 1).
 * https://oeis.org/A013724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19^(2*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 19;
    idx_t  exp   = 2*n + 1;               /* exponent 2*n+1 */

    while (exp != 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    return result;
}
