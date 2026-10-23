/*
 * A100041  a(n) = 2*n^2 + n - 7.
 * https://oeis.org/A100041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n^2 + n - 7
 * Range:     i8 n <= 7, i16 n <= 127, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    #ifdef __SIZEOF_INT128__
        __int128 tmp = ( __int128) nseq;
        tmp = 2 * tmp * tmp + tmp - 7;
        return (seq_t) tmp;
    #else
        return 2 * nseq * nseq + nseq - (seq_t)7;
    #endif
}
