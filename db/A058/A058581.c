/*
 * A058581  a(n) = (4*n^2 + 2*n - 3)*(2*n - 1)*n/3.
 * https://oeis.org/A058581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^2 + 2*n - 3)*(2*n - 1)*n/3
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Promote index to seq_t for safe arithmetic */
    seq_t ns = (seq_t)n;

    /* Compute numerator in parts to avoid intermediate sign issues */
    seq_t t1 = ns * ns * 4 + ns * 2 - (seq_t)3;  /* 4*n^2 + 2*n - 3 */
    seq_t t2 = ns * 2 - (seq_t)1;                /* 2*n - 1 */

    /* Full product */
    seq_t prod = t1 * t2 * ns;

    /* Exact division by 3 */
    return prod / (seq_t)3;
}
