/*
 * A081895  Second binomial transform of binomial(n+3, 3).
 * https://oeis.org/A081895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is defined by the linear recurrence a(n) = 12·a(n‑1) –
 *            54·a(n‑2) + 108·a(n‑3) – 81·a(n‑4) (n ≥ 4) with initial values
 *            a(0)=1, a(1)=6, a(2)=30, a(3)=136
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four indices trivially.  */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)30;
    if (n == 3) return (seq_t)136;

    /* Recurrence uses only the previous four terms; keep them in a
       circular buffer (four variables).  */
    seq_t a0 = (seq_t)1;   /* a(n-4)   */
    seq_t a1 = (seq_t)6;   /* a(n-3)   */
    seq_t a2 = (seq_t)30;  /* a(n-2)   */
    seq_t a3 = (seq_t)136; /* a(n-1)   */
    seq_t ai;              /* current a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)12 * a3
            - (seq_t)54 * a2
            + (seq_t)108 * a1
            - (seq_t)81 * a0;
        /* shift the window for next iteration.  */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
