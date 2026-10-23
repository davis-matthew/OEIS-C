/*
 * A041120  Numerators of continued fraction convergents to sqrt(69).
 * https://oeis.org/A041120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041120: Numerators of continued fraction convergents to sqrt(69).
   Recurrence:
   Let p(-2)=0, p(-1)=1.
   The partial quotients are a(0)=8,
   a(1)=3, a(2)=3, a(3)=1, a(4)=4, a(5)=1, a(6)=3, a(7)=3, a(8)=16,
   and then repeat every 8 terms.
   Then p(n)=a(n)*p(n-1)+p(n-2). */

seq_t a(idx_t n)
{
    /* period of partial quotients after the initial term a(0)=8 */
    static const seq_t period[8] = {3,3,1,4,1,3,3,16};

    seq_t prev2 = 0;   /* p(-2) */
    seq_t prev1 = 1;   /* p(-1) */
    seq_t curr  = 0;   /* placeholder for p(n) */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t a_i;
        if (i == 0)
            a_i = 8;                           /* a(0) */
        else
            a_i = period[(i - 1) % 8];         /* periodic a(i) for i>=1 */

        curr = a_i * prev1 + prev2;            /* p(i) */

        prev2 = prev1;  /* shift for next iteration */
        prev1 = curr;
    }

    return curr;   /* represents a(n) */
}
