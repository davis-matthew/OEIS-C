/*
 * A215627  Number of terms in 9th derivative of a function composed with
 *          itself n times.
 * https://oeis.org/A215627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n+1)*(n^6+75*n^5+1703*n^4+13697*n^3+29472*n^2-30788*n+6000)/40320
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 29, u64 n <= 954, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset = 1; we do not need a(0) */
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;
    seq_t n3 = n2 * nseq;
    seq_t n4 = n3 * nseq;
    seq_t n5 = n4 * nseq;
    seq_t n6 = n5 * nseq;

    /* compute the polynomial part */
    seq_t poly = n6
               + (seq_t)75  * n5
               + (seq_t)1703 * n4
               + (seq_t)13697 * n3
               + (seq_t)29472 * n2
               - (seq_t)30788 * nseq
               + (seq_t)6000;

    seq_t result = 1;
    idx_t c2 = 7, c3 = 2, c5 = 1, c7 = 1;   /* prime exponents of 40320 */
    seq_t factors[3] = { nseq, nseq + 1, poly };

    for (int i = 0; i < 3; ++i) {
        seq_t f = factors[i];

        /* cancel powers of 2 */
        while (c2 > 0 && (f & 1) == 0) {
            f >>= 1;
            --c2;
        }
        /* cancel powers of 3 */
        while (c3 > 0 && f % 3 == 0) {
            f /= 3;
            --c3;
        }
        /* cancel powers of 5 */
        while (c5 > 0 && f % 5 == 0) {
            f /= 5;
            --c5;
        }
        /* cancel powers of 7 */
        while (c7 > 0 && f % 7 == 0) {
            f /= 7;
            --c7;
        }

        result *= f;
    }

    /* the remaining dividend is now the exact value of a(n) */
    return result;
}
