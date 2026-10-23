/*
 * A028179  Expansion of 1/((1-5*x)*(1-6*x)*(1-11*x)*(1-12*x)).
 * https://oeis.org/A028179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*12^(n+3)-7*11^(n+3)+7*6^(n+3)-5^(n+4))/210 Recurrence:
 *            a(0)=1, a(1)=34, a(n) = 6^(n+1) - 5^(n+1) + 23*a(n-1) -
 *            132*a(n-2) (n >= 2)
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

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)34;  /* a(1) */
    seq_t pow6 = (seq_t)216;    /* 6^3, needed for n = 2 */
    seq_t pow5 = (seq_t)125;    /* 5^3, needed for n = 2 */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* recurrence: a(i) = 6^(i+1) - 5^(i+1) + 23*a(i-1) - 132*a(i-2) */
        seq_t term = pow6 - pow5;                     /* 6^(i+1)-5^(i+1) */
        term = term + (23 * a_prev1) - (132 * a_prev2);
        cur = term;

        /* shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = cur;
        pow6 = pow6 * (seq_t)6;
        pow5 = pow5 * (seq_t)5;
    }

    return a_prev1;
}
