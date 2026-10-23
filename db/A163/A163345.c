/*
 * A163345  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)+5*a(n-2)+5*a(n-3)+5*a(n-4)-15*a(n-5); Initial
 *            values: a(0)=1, a(1)=7, a(2)=42, a(3)=252, a(4)=1512, a(5)=9051
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {1, 7, 42, 252, 1512, 9051};
    if (n < 6)
        return init[n];

    /* t1..t5 hold a(n-5) .. a(n-1) for the current iteration */
    seq_t t1 = init[1];   /* a(1) */
    seq_t t2 = init[2];   /* a(2) */
    seq_t t3 = init[3];   /* a(3) */
    seq_t t4 = init[4];   /* a(4) */
    seq_t t5 = init[5];   /* a(5) */
    seq_t an = 0;

    for (idx_t i = 6; i <= n; ++i) {
        an = 5 * t5 + 5 * t4 + 5 * t3 + 5 * t2 - 15 * t1;
        /* shift the window one step to the right */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
        t5 = an;
    }
    return an;
}
