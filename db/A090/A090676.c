/*
 * A090676  a(n) = round(n^3/12) - floor(n/4)*floor((n+2)/4).
 * https://oeis.org/A090676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n^3/12) - floor(n/4)*floor((n+2)/4)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t before multiplicative operations  */
    seq_t nseq = (seq_t)n;
    seq_t sq   = nseq * nseq;          /* n^2   */
    seq_t cube = sq   * nseq;          /* n^3   */

    /* round(n^3/12)  ->  floor((n^3 + 6)/12)  (0.5*12 = 6)   */
    seq_t r = (cube + (seq_t)6) / (seq_t)12;

    /* floor(n/4) and floor((n+2)/4) */
    seq_t f1 = (seq_t)(n / 4);
    seq_t f2 = (seq_t)((n + 2) / 4);

    seq_t prod = f1 * f2;

    return r - prod;
}
