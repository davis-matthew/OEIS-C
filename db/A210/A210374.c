/*
 * A210374  Number of 2 X 2 matrices with all terms in {0,1,...,n} and (sum of
 *          terms) = n+2.
 * https://oeis.org/A210374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 0
 * Range:     u8 n <= 3, u16 n <= 36, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula: a(n) = (n^3 + 12*n^2 + 47*n - 36) / 6  for n > 0 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;
    seq_t n_cube = n_sq * n_seq;
    seq_t numerator = n_cube + 12 * n_sq + 47 * n_seq - 36;
    return numerator / 6;
}
