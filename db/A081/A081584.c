/*
 * A081584  Fourth row of Pascal-(1,2,1) array A081577.
 * https://oeis.org/A081584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (8 + 6*n + 81*n^2 - 18*n^3 + 27*n^4) / 8
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 141, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a direct polynomial evaluation. */
seq_t a(idx_t n)
{
    seq_t sn = n;            /* convert index to seq_t */
    seq_t sn2 = sn * sn;     /* n^2 */
    seq_t sn3 = sn2 * sn;    /* n^3 */
    seq_t sn4 = sn3 * sn;    /* n^4 */
    seq_t res = 27 * sn4     /* 27*n^4 */
              - 18 * sn3     /* -18*n^3 */
              + 81 * sn2     /* +81*n^2 */
              + 6  * sn      /* +6*n */
              + 8;           /* +8 */
    return res / 8;          /* exact division by 8 */
}
