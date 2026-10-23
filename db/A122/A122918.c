/*
 * A122918  Expansion of (1+x)^2/(1+x+x^2)^2.
 * https://oeis.org/A122918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 193, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A122918 : Expansion of (1+x)^2/(1+x+x^2)^2
 * Recurrence: a(0)=1, a(1)=0, a(2)=-2, a(3)=2
 * For n ≥ 4:  a(n) = -(2*a(n-1) + 3*a(n-2) + 2*a(n-3) + a(n-4))   */
seq_t a(idx_t n)
{
    /* Handle the first few values explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)-2;
    if (n == 3) return (seq_t)2;

    /* Iterate from 4 up to n, keeping the last four terms */
    seq_t t0 = (seq_t)1;   /* a(n-4) */
    seq_t t1 = (seq_t)0;   /* a(n-3) */
    seq_t t2 = (seq_t)-2;  /* a(n-2) */
    seq_t t3 = (seq_t)2;   /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = -(2*a(i-1) + 3*a(i-2) + 2*a(i-3) + a(i-4)) */
        cur = -((seq_t)(2 * t3 + 3 * t2 + 2 * t1 + t0));

        /* Shift the window of last four terms */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = cur;
    }

    return cur;
}
