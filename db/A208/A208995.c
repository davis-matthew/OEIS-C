/*
 * A208995  Number of 4-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero and first differences in -n..n.
 * https://oeis.org/A208995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (12 + 8*n + 6*n^2 + 4*n^3) / 12 for n even; a(n) = (6 +
 *            8*n + 6*n^2 + 4*n^3) / 12 for n odd
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1; safe guard */
    seq_t N   = (seq_t)n;
    seq_t nsq = N * N;
    seq_t ncu = nsq * N;
    seq_t num;
    if (n & 1U) {                       /* odd n */
        num = 4 * ncu + 6 * nsq + 8 * N + 6;
    } else {                            /* even n */
        num = 4 * ncu + 6 * nsq + 8 * N + 12;
    }
    return num / 12;
}
