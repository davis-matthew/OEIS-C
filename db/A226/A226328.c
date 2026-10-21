/*
 * A226328  a(0)=1, a(1)=-2; a(n+2) = a(n+1) + a(n) + (period 3: repeat 3, 1,
 *          -1).
 * https://oeis.org/A226328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 45, i64 n <= 91, i128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A226328: a(0)=1, a(1)=-2; a(n+2)=a(n+1)+a(n)+b(n) where
   b repeats 3, 1, -1.  (a(0)=1, a(1)=-2). */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)-2;  /* a(1) */
    seq_t an = 0;          /* current term */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t add;
        switch ((i - 2) % 3U)   /* i-2 rounds 0,1,2,... */
        {
            case 0: add = (seq_t)3;   break;
            case 1: add = (seq_t)1;   break;
            default: add = (seq_t)-1; break;
        }

        an = a1 + a0 + add;   /* a(i) = a(i-1)+a(i-2)+add */

        /* shift for the next iteration */
        a0 = a1;
        a1 = an;
    }

    return an;
}
