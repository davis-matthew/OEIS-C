/*
 * A020567  Expansion of 1/((1-5x)(1-10x)(1-11x)).
 * https://oeis.org/A020567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*5^n/6 -20*10^n +121*11^n/6 (recurrence: a(0)=1,
 *            a(1)=26, a(2)=461, a(n)=26*a(n-1)-215*a(n-2)+550*a(n-3) for n>2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)461;

    seq_t prev3 = (seq_t)1;      /* a(0) */
    seq_t prev2 = (seq_t)26;     /* a(1) */
    seq_t prev1 = (seq_t)461;    /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)26 * prev1 - (seq_t)215 * prev2 + (seq_t)550 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
