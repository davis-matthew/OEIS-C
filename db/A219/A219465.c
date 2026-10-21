/*
 * A219465  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          columns, 0..3 n X 2 array.
 * https://oeis.org/A219465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^8 + 16 n^7 + 42 n^6 + 1792 n^5 - 231 n^4 + 31024 n^3 +
 *            30428 n^2 + 17568 n) / 20160 for n >= 1
 * Range:     u8 none, u16 none, u32 n <= 13, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; offset is 1 */

    seq_t n_seq = (seq_t)n;
    seq_t pow[9];
    pow[0] = 1;
    for (idx_t i = 1; i <= 8; ++i)
        pow[i] = pow[i - 1] * n_seq;

    seq_t term_n8 = pow[8];
    seq_t term_n7 = 16 * pow[7];
    seq_t term_n6 = 42 * pow[6];
    seq_t term_n5 = 1792 * pow[5];
    seq_t term_n4 = 231 * pow[4];
    seq_t term_n3 = 31024 * pow[3];
    seq_t term_n2 = 30428 * pow[2];
    seq_t term_n1 = 17568 * pow[1];

    seq_t pos = term_n8 + term_n7 + term_n6 + term_n5
               + term_n3 + term_n2 + term_n1;
    seq_t neg = term_n4;

    seq_t numer = pos - neg;
    const seq_t denom = 20160;
    return numer / denom;
}
