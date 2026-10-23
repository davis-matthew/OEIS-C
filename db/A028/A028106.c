/*
 * A028106  Expansion of 1/((1-3x)(1-9x)(1-10x)(1-12x)).
 * https://oeis.org/A028106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - 411*a(n-2) + 2034*a(n-3) - 3240*a(n-4) with
 *            a(0)=1, a(1)=34, a(2)=745, a(3)=13390
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)34;
    if (n == 2) return (seq_t)745;
    if (n == 3) return (seq_t)13390;

    seq_t a0 = (seq_t)1;        /* a(n-4)   */
    seq_t a1 = (seq_t)34;       /* a(n-3)   */
    seq_t a2 = (seq_t)745;      /* a(n-2)   */
    seq_t a3 = (seq_t)13390;    /* a(n-1)   */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)34 * a3
            - (seq_t)411 * a2
            + (seq_t)2034 * a1
            - (seq_t)3240 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
