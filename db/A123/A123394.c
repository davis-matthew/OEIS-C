/*
 * A123394  Values X satisfying the equation 7(X-Y)^4-8XY=0, where X>=Y.
 * https://oeis.org/A123394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0 a(1) = 64 a(2) = 54000 a(3) = 48387776 For n ≥ 4: a(n)
 *            = 928·a(n‑1) – 26942·a(n‑2) + 928·a(n‑3) – a(n‑4) (The
 *            recurrence is used directly; no memoisation needed for n ≤
 *            10000.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four indices explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)64;
    if (n == 2) return (seq_t)54000;
    if (n == 3) return (seq_t)48387776;

    /* Sliding‑window recurrence using only the last four values. */
    seq_t a0 = (seq_t)0;                      /* a(n‑4)   */
    seq_t a1 = (seq_t)64;                     /* a(n‑3)   */
    seq_t a2 = (seq_t)54000;                   /* a(n‑2)   */
    seq_t a3 = (seq_t)48387776;                /* a(n‑1)   */
    seq_t cur = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i) {
        /* cur = 928·a3 – 26942·a2 + 928·a1 – a0 */
        cur = (seq_t)928  * a3
            - (seq_t)26942 * a2
            + (seq_t)928  * a1
            - a0;
        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }
    return a3;
}
