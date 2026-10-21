/*
 * A228470  a(n) = 6*a(n-2) + a(n-4), where a(0) = 3, a(1) = 11, a(2) = 18,
 *          a(3) = 68.
 * https://oeis.org/A228470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) + a(n-4), a(0)=3, a(1)=11, a(2)=18, a(3)=68
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)3;
    if (n == (idx_t)1) return (seq_t)11;
    if (n == (idx_t)2) return (seq_t)18;
    if (n == (idx_t)3) return (seq_t)68;

    /* Working variables for the last four values:
       t0 = a(n-4), t1 = a(n-3), t2 = a(n-2), t3 = a(n-1)   */
    seq_t t0 = (seq_t)3;   /* a(0)   */
    seq_t t1 = (seq_t)11;  /* a(1)   */
    seq_t t2 = (seq_t)18;  /* a(2)   */
    seq_t t3 = (seq_t)68;  /* a(3)   */
    seq_t res;

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        /* a(i) = 6*a(i-2) + a(i-4) */
        res = 6 * t2 + t0;

        /* shift window forward for next iteration */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = res;
    }
    return res;
}
