/*
 * A199316  a(n) = 11*5^n+1.
 * https://oeis.org/A199316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*5^n + 1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 5^n in an unsigned type that matches seq_t. */
    seq_t pow5 = 1;                 /* 5^0 = 1 */
    idx_t i;
    for (i = 0; i < n; ++i) {
        pow5 *= (seq_t)5;
    }
    /* Multiply by 11 and add 1. */
    return pow5 * (seq_t)11 + (seq_t)1;
}
