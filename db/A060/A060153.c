/*
 * A060153  Duplicate of A055010.
 * https://oeis.org/A060153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3*2^(n-1) - 1 for n ≥ 1, a(0) = 0 (OEIS A060153)
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;
    seq_t val = 2;                /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        val = 2 * val + 1;        /* a(i) = 2*a(i-1) + 1 */
    }
    return val;
}
