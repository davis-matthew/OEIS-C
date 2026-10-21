/*
 * A099452  An Alexander sequence for the knot 7_7.
 * https://oeis.org/A099452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=5, a(2)=16, a(3)=40, a(4)=79;
 *            a(n)=5*a(n‑1)-9*a(n‑2)+5*a(n‑3)-a(n‑4) for n≥5
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 23, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small n handled directly */
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 16;
    if (n == 3) return 40;

    /* iterative computation of the recurrence */
    seq_t a0 = 1;   /* a(n‑4) */
    seq_t a1 = 5;   /* a(n‑3) */
    seq_t a2 = 16;  /* a(n‑2) */
    seq_t a3 = 40;  /* a(n‑1) */
    seq_t next;
    for (idx_t i = 4; i <= n; ++i)
    {
        if (i == 4)
            /* special adjustment for the 4th term: -1 on the right‑hand side */
            next = 5 * a3 - 9 * a2 + 5 * a1 - a0 - 1;
        else
            next = 5 * a3 - 9 * a2 + 5 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
