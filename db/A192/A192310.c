/*
 * A192310  1-sequence of reduction of (3n-1) by x^2 -> x+1.
 * https://oeis.org/A192310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + a(n-4) + a(n-5) with
 *            a(1)=0, a(2)=5, a(3)=13, a(4)=35, a(5)=77
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 5)
    {
        static const seq_t init[5] = {
            (seq_t)0,
            (seq_t)5,
            (seq_t)13,
            (seq_t)35,
            (seq_t)77
        };
        return init[n - 1];
    }

    seq_t a1 = (seq_t)0;
    seq_t a2 = (seq_t)5;
    seq_t a3 = (seq_t)13;
    seq_t a4 = (seq_t)35;
    seq_t a5 = (seq_t)77;

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t next = (seq_t)3 * a5;
        next += a2;
        next += a1;
        next -= a4;
        next -= (seq_t)3 * a3;

        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;
}
