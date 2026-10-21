/*
 * A209264  a(n) = 1 + 2*n^2 + 3*n^3 + 4*n^4 +5*n^5 + 6*n^6.
 * https://oeis.org/A209264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*n^2 + 3*n^3 + 4*n^4 + 5*n^5 + 6*n^6
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 1205,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t n_t = (seq_t)n;
    seq_t n2 = (seq_t)((__int128)n_t * n_t);
    seq_t n3 = (seq_t)((__int128)n2 * n_t);
    seq_t n4 = (seq_t)((__int128)n3 * n_t);
    seq_t n5 = (seq_t)((__int128)n4 * n_t);
    seq_t n6 = (seq_t)((__int128)n5 * n_t);

    seq_t result = (seq_t)1;
    result += (seq_t)2 * n2;
    result += (seq_t)3 * n3;
    result += (seq_t)4 * n4;
    result += (seq_t)5 * n5;
    result += (seq_t)6 * n6;
    return result;
}
