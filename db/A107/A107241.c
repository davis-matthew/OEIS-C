/*
 * A107241  Sum of squares of first n tetranacci numbers (A000288).
 * https://oeis.org/A107241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A107241
 *  Formula: a(n) = Sum_{i=1..n} T_i^2
 *  where T_1 = T_2 = T_3 = T_4 = 1,
 *        T_k   = T_{k-1} + T_{k-2} + T_{k-3} + T_{k-4}   (k ≥ 5)
 *  a(n) is the sum of squares of the first n tetranacci numbers.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0)
        return 0;

    /* First four tetranacci numbers are all 1 */
    if (n <= 4) {
        /* sum of 1^2 added n times */
        return (seq_t)n;
    }

    /* Compute the recurrence for T_k and accumulate the squares */
    seq_t a1 = 1, a2 = 1, a3 = 1, a4 = 1;  /* T_{k-4}, T_{k-3}, T_{k-2}, T_{k-1} */
    /* Sum the squares of the first four terms */
    sum = 4;   /* 1^2 + 1^2 + 1^2 + 1^2 */

    idx_t i;
    for (i = 5; i <= n; ++i) {
        seq_t cur = a1 + a2 + a3 + a4;   /* T_k */
        sum += cur * cur;                /* add square */
        /* shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }

    return sum;
}
