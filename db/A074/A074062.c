/*
 * A074062  Reflected (see A074058) pentanacci numbers A074048.
 * https://oeis.org/A074062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1)-a(n-2)-a(n-3)-a(n-4)+a(n-5); a(0) = 5, a(1) =
 *            a(2) = a(3) = a(4) = -1
 * Range:     i8 n <= 20, i16 n <= 51, i32 n <= 104, i64 n <= 215,
 *            i128 n <= 437, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first five values directly */
    if (n == 0) return (seq_t)5;
    if (n <= 4) return (seq_t)-1;

    /* sliding‑window recurrence computation */
    seq_t t0 = (seq_t)5;          /* a(n-5) */
    seq_t t1 = (seq_t)-1;         /* a(n-4) */
    seq_t t2 = (seq_t)-1;         /* a(n-3) */
    seq_t t3 = (seq_t)-1;         /* a(n-2) */
    seq_t t4 = (seq_t)-1;         /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = -(t1 + t2 + t3 + t4) + t0; /* a(i) */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = next;
    }

    return t4;  /* a(n) */
}
