/*
 * A060483  Number of 5-block tricoverings of an n-set.
 * https://oeis.org/A060483
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n - 15*4^n + 45*2^n - 40)/120. Recurrence (valid for
 *            n ≥ 5): a(n) = 17*a(n‑1) – 84*a(n‑2) + 148*a(n‑3) – 80*a(n‑4).
 *            Initial values: a(0)=a(1)=a(2)=0, a(3)=3, a(4)=57
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n < 3)      return (seq_t)0;
    if (n == 3)     return (seq_t)3;
    if (n == 4)     return (seq_t)57;
    if (n == 5)     return (seq_t)717;   /* may be computed by the loop anyway */

    /* Use the recurrence for n ≥ 5 */
    seq_t a0 = (seq_t)0;   /* a(n-4) for current iteration */
    seq_t a1 = (seq_t)0;   /* a(n-3)   */
    seq_t a2 = (seq_t)0;   /* a(n-2)   */
    seq_t a3 = (seq_t)3;   /* a(n-1)   */
    seq_t a4 = (seq_t)57;  /* a(n)     */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = (seq_t)17 * a4 
                   - (seq_t)84 * a3 
                   + (seq_t)148 * a2 
                   - (seq_t)80 * a1;

        /* shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
