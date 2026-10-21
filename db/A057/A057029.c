/*
 * A057029  Central column of arrays in A057027 and A057028.
 * https://oeis.org/A057029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^2-3*n+2)/2 for even n, (4*n^2-5*n+3)/2 for odd n
 * Range:     u8 n <= 8, u16 n <= 137, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t nn = N * N;
    if (n & 1) {            /* odd n */
        seq_t num = (seq_t)4 * nn;   /* 4*n^2 */
        num = num - (seq_t)5 * N;    /* -5*n */
        num = num + (seq_t)3;        /* +3 */
        return num / 2;
    } else {                /* even n */
        seq_t num = (seq_t)4 * nn;   /* 4*n^2 */
        num = num - (seq_t)3 * N;    /* -3*n */
        num = num + (seq_t)2;        /* +2 */
        return num / 2;
    }
}
