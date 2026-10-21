/*
 * A098706  a(n) = 2*A076218(n).
 * https://oeis.org/A098706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1)-35*a(n-2)+a(n-3) for n > 3 with initial values
 *            a(0)=0, a(1)=2, a(2)=10, a(3)=290. This gives OEIS sequence
 *            A098706
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first few terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)290;

    /* Sufficient to keep only the last three values */
    seq_t t1 = (seq_t)2;          /* a(n-3) */
    seq_t t2 = (seq_t)10;         /* a(n-2) */
    seq_t t3 = (seq_t)290;        /* a(n-1) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        /* Apply the recurrence: a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3) */
        cur = (seq_t)35 * t3 - (seq_t)35 * t2 + t1;

        /* Shift the window for next iteration */
        t1 = t2;
        t2 = t3;
        t3 = cur;
    }

    return cur;
}
