/*
 * A024047  a(n) = 4^n - n^11.
 * https://oeis.org/A024047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= 4;

    seq_t n_seq = (seq_t)n;
    seq_t pown = 1;
    for (idx_t i = 0; i < 11; ++i)
        pown *= n_seq;

    return pow4 - pown;
}
