/*
 * A099454  A Chebyshev transform of A099453 associated to the knot 8_12.
 * https://oeis.org/A099454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is OEIS A099454
    Recurrence:
        a(0)=1, a(1)=7, a(2)=37, a(3)=175
        a(n)=7*a(n-1)-13*a(n-2)+7*a(n-3)-a(n-4)   for n>=4   */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)37;
    if (n == 3) return (seq_t)175;

    /* Sliding window for the last four values:
         a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1)   */
    seq_t a0 = 1, a1 = 7, a2 = 37, a3 = 175;
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute the right‑hand side using only + and - with
           intermediate unsigned arithmetic.  For every i>=4 the
           intermediate subtractions are positive; otherwise the
           sequence would not be monotonic. */
        seq_t t1 = a3;  t1 *= 7;          /* 7*a(n-1)           */
        seq_t t2 = a2;  t2 *= 13;         /* 13*a(n-2)          */
        seq_t t3 = a1;  t3 *= 7;          /* 7*a(n-3)           */

        next = t1;
        next -= t2;          /* 7*a(n-1) - 13*a(n-2) */
        next += t3;          /* + 7*a(n-3)           */
        next -= a0;          /* - a(n-4)             */

        /* Shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
