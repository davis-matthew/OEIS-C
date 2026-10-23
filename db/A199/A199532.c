/*
 * A199532  Number of -n..n arrays x(0..4) of 5 elements with zero sum and no
 *          two consecutive zero elements.
 * https://oeis.org/A199532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (115*n^4 + 230*n^3 + 41*n^2 - 2*n) / 12 This matches the
 *            provided empirical polynomial for the sequence A199532
 * Range:     u8 none, u16 n <= 4, u32 n <= 81, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;      /* the sequence is defined for n >= 1 */

    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;        /* n^2 */
    seq_t n3 = n2 * N;       /* n^3 */
    seq_t n4 = n2 * n2;      /* n^4 */

    seq_t numer = (seq_t)115 * n4 + (seq_t)230 * n3
                  + (seq_t)41 * n2 - (seq_t)2 * N;

    return numer / (seq_t)12;
}
