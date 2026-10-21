/*
 * A064756  a(n) = n*10^n - 1.
 * https://oeis.org/A064756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*10^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 10^n */
    seq_t ten_pow = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        ten_pow *= (seq_t)10;
    }

    /* Compute n * 10^n - 1 */
    seq_t result = (seq_t)n * ten_pow;
    result -= (seq_t)1;
    return result;
}
