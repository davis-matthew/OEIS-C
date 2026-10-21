/*
 * A055802  a(n) = T(n,n-2), array T as in A055801.
 * https://oeis.org/A055802
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 2,3; for even n > 2: a(n) = (n^2 - 2n) / 8; for
 *            odd n > 2: a(n) = (n^2 - 4n + 11) / 8
 * Range:     u8 n <= 17, u16 n <= 314, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 2 || n == 3) {
        /* a(2) = 1, a(3) = 1 */
        return (seq_t)1;
    }

    seq_t nseq = (seq_t)n;
    /* compute the value depends on parity of n */
    if ((n & 1) == 0) {          /* even n */
        seq_t term = nseq * nseq - (seq_t)2 * nseq;
        return term / (seq_t)8;
    } else {                      /* odd n */
        seq_t term = nseq * nseq - (seq_t)4 * nseq + (seq_t)11;
        return term / (seq_t)8;
    }
}
