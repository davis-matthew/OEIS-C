/*
 * A100160  Structured disdyakis triacontahedral numbers (vertex structure 5).
 * https://oeis.org/A100160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (116*n^3 - 168*n^2 + 58*n)/6
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t result = 1;          /* a(1) */
    if (n == 1) return result;

    for (idx_t i = 1; i < n; ++i) {
        seq_t i_seq = (seq_t)i;          /* convert to seq_t */
        seq_t term = i_seq * i_seq;      /* i^2          */
        term *= 58;                       /* 58*i^2      */
        term += 2 * i_seq;                /* plus 2*i     */
        term += 1;                        /* plus 1       */
        result += term;                   /* accumulate   */
    }
    return result;
}
