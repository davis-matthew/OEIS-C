/*
 * A145209  Numbers x such that (x+67)^3-x^3 is a square.
 * https://oeis.org/A145209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 1030190*a(n+1)-a(n)+34511298 with initial terms
 *            a(1)=9782, a(2)=10111839727
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset */
    if (n == 0)
        return (seq_t)0;              /* undefined below offset; safe fallback */

    /* Base cases */
    if (n == 1)
        return (seq_t)9782;
    if (n == 2)
        return (seq_t)10111839727;

    /* Pre‑compute constants once */
    const seq_t C1 = (seq_t)1030190;   /* coefficient of a(n+1) */
    const seq_t C2 = (seq_t)34511298;  /* constant term */

    /* Sequential recurrence: a(n+1) = 1030190*a(n) - a(n-1) + 34511298 */
    seq_t a_prev1 = (seq_t)9782;          /* a(n-1)  for n=3 */
    seq_t a_prev2 = (seq_t)10111839727;   /* a(n)    for n=3 */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t temp = a_prev2 * C1;  /* 1030190*a(n)   */
        temp += C2;                /* + 34511298     */
        temp -= a_prev1;           /* - a(n-1)       */

        a_prev1 = a_prev2;         /* shift window    */
        a_prev2 = temp;            /* current value   */
    }

    return a_prev2;                  /* a(n) */
}
