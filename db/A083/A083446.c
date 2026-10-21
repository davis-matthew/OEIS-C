/*
 * A083446  a(n) = ((10^n - 1) - 9^n)/9.
 * https://oeis.org/A083446
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = ((10^n - 1) - 9^n)/9; Recurrence: a(n) = 20*a(n-1) -
 *            109*a(n-2) + 90*a(n-3); Initial values: a(2)=2, a(3)=30,
 *            a(4)=382
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)2;
    if (n == 3)
        return (seq_t)30;
    if (n == 4)
        return (seq_t)382;

    seq_t a2 = (seq_t)2;
    seq_t a3 = (seq_t)30;
    seq_t a4 = (seq_t)382;
    seq_t next;

    for (idx_t i = 5; i <= n; ++i)
    {
        next = 20 * a4;          /* 20*a(n-1) */
        next -= 109 * a3;        /* subtract 109*a(n-2) */
        next += 90 * a2;         /* add 90*a(n-3) */
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
