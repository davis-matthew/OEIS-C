/*
 * A163615  a(n) = ((1 + 3*sqrt(2))*(4 + sqrt(2))^n + (1 - 3*sqrt(2))*(4 -
 *          sqrt(2))^n)/2.
 * https://oeis.org/A163615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163615: a(n) = 8*a(n-1) - 14*a(n-2), a(0) = 1, a(1) = 10 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)10;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)8 * prev1 - (seq_t)14 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
