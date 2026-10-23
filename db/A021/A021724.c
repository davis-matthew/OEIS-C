/*
 * A021724  Expansion of 1/((1-x)(1-3x)(1-10x)(1-12x)).
 * https://oeis.org/A021724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=26, a(2)=465, a(3)=7150;
 *            a(n)=26*a(n-1)-211*a(n-2)+546*a(n-3)-360*a(n-4) for n>=4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four base cases. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)26;
    if (n == 2u) return (seq_t)465;
    if (n == 3u) return (seq_t)7150;

    /* Initialize the last four terms. */
    seq_t a0 = (seq_t)1;     /* a(n-4)  */
    seq_t a1 = (seq_t)26;    /* a(n-3)  */
    seq_t a2 = (seq_t)465;   /* a(n-2)  */
    seq_t a3 = (seq_t)7150;  /* a(n-1)  */

    seq_t ai = a3;           /* placeholder for a(n) */
    for (idx_t i = 4u; i <= n; ++i)
    {
        /* Compute a(n) = 26*a3 - 211*a2 + 546*a1 - 360*a0 */
        ai = 26 * a3 + 546 * a1;      /* first two positive terms */
        ai -= 211 * a2;               /* subtract the middle term   */
        ai -= 360 * a0;               /* subtract the last term     */

        /* Shift the window: drop a0, move others forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return ai;
}
