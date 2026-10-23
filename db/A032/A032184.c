/*
 * A032184  a(n) = 2^n*(n-1)! for n > 1, a(1) = 1.
 * https://oeis.org/A032184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (n-1)! (a(1) = 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 1 directly for n == 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Compute (n-1)! */
    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n - 1; ++i) {
        result *= (seq_t)i;
    }

    /* Multiply by 2^n */
    for (idx_t i = 0; i < n; ++i) {
        result *= (seq_t)2;
    }

    return result;
}
