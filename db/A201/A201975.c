/*
 * A201975  Number of n X 2 0..3 arrays with rows and columns
 *          lexicographically nondecreasing read forwards and nonincreasing
 *          read backwards.
 * https://oeis.org/A201975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/6)*n^3 + 7*n^2 + (23/6)*n - 7
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer form: a(n) = (n^3 + 42*n^2 + 23*n - 42) / 6 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    /* compute numerator: n^3 + 42*n^2 + 23*n - 42 */
    seq_t num = n_seq * n_seq * n_seq
              + 42   * n_seq * n_seq
              + 23   * n_seq
              - 42;
    return num / 6;
}
