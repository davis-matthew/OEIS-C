/*
 * A130668  Diagonal of A129819.
 * https://oeis.org/A130668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(3)=-2, a(4)=5, a(5)=-11, a(6)=23;
 *            recurrence: a(n) = -6*a(n-1) - 14*a(n-2) - 16*a(n-3) - 8*a(n-4)
 *            for n>=7
 * Range:     i8 n <= 8, i16 n <= 15, i32 n <= 30, i64 n <= 61, i128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[7] = {0, 0, 1, -2, 5, -11, 23};
    if (n < 7)
        return init[n];
    seq_t a4 = init[3];      /* a(n-4) */
    seq_t a3 = init[4];      /* a(n-3) */
    seq_t a2 = init[5];      /* a(n-2) */
    seq_t a1 = init[6];      /* a(n-1) */
    seq_t next;
    for (idx_t i = 7; i <= n; ++i)
    {
        next = (seq_t)-6 * a1
             + (seq_t)-14 * a2
             + (seq_t)-16 * a3
             + (seq_t)-8  * a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = next;
    }
    return a1;
}
