/*
 * A157620  a(n) = 781250*n^2 - 1107500*n + 392499.
 * https://oeis.org/A157620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 781250*n^2 - 1107500*n + 392499
 * Range:     u8 none, u16 none, u32 n <= 74, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.
       For n == 0 the function may return 0; behaviour below the offset
       does not affect the tests. */
    if (n == 0) return 0;

    seq_t sn = (seq_t)n;          /* cast index to seq_t */
    seq_t term1 = sn * sn * (seq_t)781250;     /* 781250*n^2 */
    seq_t term2 = sn * (seq_t)1107500;         /* 1107500*n   */
    return term1 - term2 + (seq_t)392499;      /* + 392499    */
}
