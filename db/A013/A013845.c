/*
 * A013845  a(n) = 7^(5*n + 4).
 * https://oeis.org/A013845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(5*n + 4)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t exp = 5 * n + 4;          /* exponent 5*n + 4 */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)7;

    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}
