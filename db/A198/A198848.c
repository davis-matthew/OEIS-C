/*
 * A198848  a(n) = 11*6^n - 1.
 * https://oeis.org/A198848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*6^n - 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses exponentiation by squaring to compute 6^n. */

seq_t a(idx_t n)
{
    /* Compute 6^n first */
    seq_t result = 1;
    seq_t base = 6;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    /* Multiply by 11 and subtract 1 */
    result = result * (seq_t)11;
    result = result - (seq_t)1;
    return result;
}
