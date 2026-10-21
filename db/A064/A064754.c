/*
 * A064754  a(n) = n*8^n - 1.
 * https://oeis.org/A064754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*8^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 8^n with exponentiation by squaring */
    seq_t pw = 1;
    seq_t base = 8;
    idx_t e = n;
    while (e > 0) {
        if (e & 1U)
            pw *= base;
        base *= base;
        e >>= 1U;
    }
    /* multiply by n and subtract 1 */
    seq_t result = (seq_t)n * pw;
    result -= 1;
    return result;
}
