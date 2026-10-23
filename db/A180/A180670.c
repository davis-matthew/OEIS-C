/*
 * A180670  a(n) = a(n-1)+a(n-2)+a(n-3)+(8*n^3-48*n^2+112*n-96)/3 with a(0)=0,
 *          a(1)=0 and a(2)=1.
 * https://oeis.org/A180670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+(8*n^3-48*n^2+112*n-96)/3 initial
 *            values: a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    seq_t a0 = 0, a1 = 0, a2 = 1;   // a(0), a(1), a(2)
    seq_t P   = 0;                  // P(2) = 0
    seq_t d   = 8;                  // d(3) = 8

    for (idx_t i = 3; i <= n; ++i)
    {
        P += d;                                 // P(i)
        seq_t ai = a0 + a1 + a2 + P;            // a(i)

        // shift the last three values for the next iteration
        a0 = a1;
        a1 = a2;
        a2 = ai;

        // Increment d to d(i+1); D(i+1) = D(i) + (16*i - 32)
        seq_t e = (seq_t)16 * i - 32;
        d += e;
    }

    return a2;
}
