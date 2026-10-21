/*
 * A077933  Expansion of (1-x)^(-1)/(1+2*x+2*x^2-x^3).
 * https://oeis.org/A077933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -a(n-1) + 3*a(n-3) - a(n-4)
 * with initial values a(0)=1, a(1)=-1, a(2)=1, a(3)=2
 * Equivalent to the expansion of (1-x)^(-1)/(1+2*x+2*x^2-x^3). */

seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t init[4] = { (seq_t)1, (seq_t)-1, (seq_t)1, (seq_t)2 };
    if (n < (idx_t)4) return init[n];

    /* buffer holding the last four computed terms */
    seq_t buf[4];
    for (int i = 0; i < 4; ++i) buf[i] = init[i];

    /* compute up to nth term iteratively */
    for (idx_t i = (idx_t)4; i <= n; ++i)
    {
        seq_t next = -buf[(i - 1) & 3] + (seq_t)3 * buf[(i - 3) & 3] - buf[(i - 4) & 3];
        buf[i & 3] = next;
    }
    return buf[n & 3];
}
