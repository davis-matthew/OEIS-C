/*
 * A207737  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 11*a(n-2) - 12*a(n-3) + 13*a(n-4) + 4*a(n-5) -
 *            a(n-6) (empirical recurrence for n > 7) with initial values:
 *            a(1)=9, a(2)=81, a(3)=361, a(4)=1849, a(5)=10201, a(6)=58081,
 *            a(7)=335241
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: series starts at n = 1 */
    if (n <= 7) {
        static const seq_t init[8] =
            {0, 9, 81, 361, 1849, 10201, 58081, 335241};
        return init[n];
    }

    /* Initialize the first seven terms. */
    seq_t a1 = 9, a2 = 81, a3 = 361, a4 = 1849, a5 = 10201,
          a6 = 58081, a7 = 335241, an = 0;

    /* Iteratively apply the recurrence up to the required n. */
    for (idx_t i = 8; i <= n; ++i) {
        an = 8 * a7 - 11 * a6 - 12 * a5 + 13 * a4 + 4 * a3 - a2;
        /* Shift the window of previous terms. */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = an;
    }

    return an;
}
