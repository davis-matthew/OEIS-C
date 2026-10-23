/*
 * A036258  Number of inequivalent strings of n digits, when 2 strings are
 *          equivalent if turning 1 upside down gives the other.
 * https://oeis.org/A036258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 15*a(n-1) - 45*a(n-2) - 75*a(n-3) + 250*a(n-4), a(0)=1,
 *            a(1)=9, a(2)=90, a(3)=945, for n>3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    switch (n) {
        case 0: return 1;
        case 1: return 9;
        case 2: return 90;
        case 3: return 945;
    }

    /* a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = 1;      /* a(0)   */
    seq_t a1 = 9;      /* a(1)   */
    seq_t a2 = 90;     /* a(2)   */
    seq_t a3 = 945;    /* a(3)   */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        /* compute an = 15*a3 - 45*a2 - 75*a1 + 250*a0
           rearranged to avoid unsigned underflow */
        seq_t sum1 = (seq_t)15 * a3 + (seq_t)250 * a0;
        seq_t sum2 = (seq_t)45 * a2 + (seq_t)75 * a1;
        an = sum1 - sum2;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
