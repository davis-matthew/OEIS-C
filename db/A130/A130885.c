/*
 * A130885  a(n) = 3n^3 - 2n^2 + n - 1.
 * https://oeis.org/A130885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^3 - 2*n^2 + n - 1 (offset 1)
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t sq  = N * N;           /* n^2   */
    seq_t cub = sq * N;          /* n^3   */
    seq_t val = (seq_t)3 * cub;  /* 3*n^3 */
    val -= (seq_t)2 * sq;        /* -2*n^2 */
    val += N;                    /* + n    */
    val -= (seq_t)1;             /* - 1    */
    return val;                  /* a(n)   */
}
