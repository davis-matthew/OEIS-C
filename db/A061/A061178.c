/*
 * A061178  Third column (m=2) of triangle A060920 (bisection of Fibonacci
 *          triangle, even part).
 * https://oeis.org/A061178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0)=1, a(1)=9, a(2)=51, a(3)=233, a(4)=942, a(5)=3522;
   for n≥6: a(n)=9*a(n-1)-30*a(n-2)+45*a(n-3)-30*a(n-4)+9*a(n-5)-a(n-6)
*/
seq_t a(idx_t n) {
    /* handle small indices explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)51;
    if (n == 3) return (seq_t)233;
    if (n == 4) return (seq_t)942;
    if (n == 5) return (seq_t)3522;

    /* sliding window of the last six values */
    seq_t v0 = (seq_t)1;   /* a(n-6)   */
    seq_t v1 = (seq_t)9;   /* a(n-5)   */
    seq_t v2 = (seq_t)51;  /* a(n-4)   */
    seq_t v3 = (seq_t)233; /* a(n-3)   */
    seq_t v4 = (seq_t)942; /* a(n-2)   */
    seq_t v5 = (seq_t)3522;/* a(n-1)   */
    seq_t vn;              /* a(n)     */

    for (idx_t i = 6; i <= n; ++i) {
        vn = 9 * v5
           - 30 * v4
           + 45 * v3
           - 30 * v2
           + 9 * v1
           - v0;
        /* shift the window forward */
        v0 = v1; v1 = v2; v2 = v3;
        v3 = v4; v4 = v5; v5 = vn;
    }
    return vn;
}
