/*
 * A042742  Numerators of continued fraction convergents to sqrt(902).
 * https://oeis.org/A042742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042742:  a(n) = 1802*a(n-2) - a(n-4)
 * Initial values:  a(0)=30, a(1)=901, a(2)=54090, a(3)=1623601
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)30;
    if (n == 1) return (seq_t)901;
    if (n == 2) return (seq_t)54090;
    if (n == 3) return (seq_t)1623601;

    /* Keep the last four computed terms:
     *  prev0 = a(i-4), prev1 = a(i-3), prev2 = a(i-2), prev3 = a(i-1)
     *  after computing a(i) we shift them for the next iteration
     */
    seq_t prev0 = (seq_t)30;           /* a(i-4) when i=4   */
    seq_t prev1 = (seq_t)901;          /* a(i-3) when i=4   */
    seq_t prev2 = (seq_t)54090;        /* a(i-2) when i=4   */
    seq_t prev3 = (seq_t)1623601;      /* a(i-1) when i=4   */
    seq_t cur   = 0;                   /* placeholder for a(i) */

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)1802 * prev2 - prev0;  /* a(i) = 1802*a(i-2) - a(i-4) */
        /* shift for next iteration */
        prev0 = prev1;
        prev1 = prev2;
        prev2 = prev3;
        prev3 = cur;
    }
    return cur;
}
