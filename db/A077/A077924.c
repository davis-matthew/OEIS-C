/*
 * A077924  Expansion of (1-x)^(-1)/(1+2*x-2*x^3).
 * https://oeis.org/A077924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n-1) + 2*a(n-2) + 2*a(n-3) - 2*a(n-4) a(0) = 1, a(1) =
 *            -1, a(2) = 3, a(3) = -3
 * Range:     i8 n <= 14, i16 n <= 24, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 0..3 */
    const seq_t base0 = (seq_t)1;
    const seq_t base1 = (seq_t)-1;
    const seq_t base2 = (seq_t)3;
    const seq_t base3 = (seq_t)-3;

    if (n < 4)
    {
        switch (n)
        {
            case 0: return base0;
            case 1: return base1;
            case 2: return base2;
            case 3: return base3;
        }
    }

    /* a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) for current i */
    seq_t a0 = base0, a1 = base1, a2 = base2, a3 = base3;

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t an = -a3 + (seq_t)2 * a2 + (seq_t)2 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;          /* a(n) */
}
