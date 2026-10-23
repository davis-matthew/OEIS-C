/*
 * A135917  a(n) = (n^6 - 30*n^4 + 45*n^3 + 206*n^2 - 576*n + 384)/6.
 * https://oeis.org/A135917
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - 30*n^4 + 45*n^3 + 206*n^2 - 576*n + 384)/6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence has offset 2; this function works for all n >= 2. */
seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t n2 = n_t * n_t;          /* n^2   */
    seq_t n3 = n2 * n_t;           /* n^3   */
    seq_t n4 = n3 * n_t;           /* n^4   */
    seq_t n5 = n4 * n_t;           /* n^5   */
    seq_t n6 = n5 * n_t;           /* n^6   */

    /* Compute the numerator as sum1 - sum2 to avoid unsigned underflow. */
    seq_t sum1 = n6 + 384 + 206 * n2 + 45 * n3;
    seq_t sum2 = 30 * n4 + 576 * n_t;
    seq_t numerator = sum1 - sum2;

    return numerator / 6;         /* Division is exact for all integer n. */
}
