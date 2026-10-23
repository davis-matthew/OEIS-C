/*
 * A008394  Crystal ball sequence for A_9 lattice.
 * https://oeis.org/A008394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in
 *            (1+x)*(1+80x+1216x^2+5840x^3+10036x^4+5840x^5+1216x^6+80x^7+x^8)/(1-x)^10
 *            Equivalent polynomial: a(n) = 2431/18144*n^9 + 2431/4032*n^8 +
 *            4433/1512*n^7 + 715/96*n^6 + 67067/4320*n^5 + 4147/192*n^4 +
 *            197329/9072*n^3 + 14465/1008*n^2 + 7129/1260*n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 14, u64 n <= 172, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom9(idx_t N)
{
    if (N < 9) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= 9; ++i)
    {
        res = res * (seq_t)(N - 9 + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Symmetric polynomial coefficients of P(x)
       P(x) = 1 + 80x + 1216x^2 + 5840x^3 + 10036x^4
              + 5840x^5 + 1216x^6 + 80x^7 + x^8   */
    static const uint16_t p[9] = {
        1,  80,  1216,  5840, 10036, 5840, 1216, 80, 1
    };
    seq_t result = 0;

    for (int j = 0; j < 9; ++j)
    {
        if (n < (idx_t)j) continue;          /* terms with negative index vanish */

        idx_t t1 = n - j + 9;                /* for binom(n-j+9,9)   */
        if (t1 >= 9)
            result += (seq_t)p[j] * binom9(t1);

        if (n >= (idx_t)(j + 1))
        {
            idx_t t2 = n - j - 1 + 9;        /* for binom(n-j-1+9,9) */
            if (t2 >= 9)
                result += (seq_t)p[j] * binom9(t2);
        }
    }

    return result;
}
