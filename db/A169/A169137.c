/*
 * A169137  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A169137 : Number of reduced words of length n in the Coxeter group
    with 28 generators S_i and relations S_i^2 = (S_i S_j)^26 = I
    The generating function is

          P(t) = t^26 + 2 t^25 + 2 t^24 + … + 2 t + 1
          Q(t) = 1 - 26 t - 26 t^2 – … – 26 t^25 + 351 t^26

    Hence for n ≥ 26

          a(n) = 26 (a(n-1)+a(n-2)+…+a(n-25)) – 351 a(n-26)

    For 0 ≤ n < 26 the coefficients are obtained by expanding
    the power series of P(t)/Q(t), which yields the starting values
    a(0)=1, a(1)=28, a(2)=756, …, a(15)=3063731695682346057852.
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* we only need to keep the last 27 values (0 … 26) */
    seq_t buf[27] = {0};
    buf[0] = (seq_t)1;          /* a(0) */

    /* pre‑compute coefficients as constants of type seq_t */
    const seq_t C26  = (seq_t)26;
    const seq_t C351 = (seq_t)351;

    size_t N = (size_t)n;       /* n is guaranteed to be < SIZE_MAX in usage */

    for (size_t i = 1; i <= N; ++i)
    {
        /* numerator coefficient p_i : 1 for i==0 or i==26, 2 otherwise, 0 for i>26 */
        seq_t p;
        if (i == 26)
            p = (seq_t)1;
        else if (i < 26)
            p = (seq_t)2;
        else
            p = (seq_t)0;

        /* compute a(i) using the recurrence derived from P(t)/Q(t) */
        seq_t sum = p;                     /* start with p_i */

        /* add 26 * a(i-j) for j=1…min(i,25) */
        size_t up = (i < 26) ? i : 25;
        for (size_t j = 1; j <= up; ++j)
            sum += C26 * buf[(i - j) % 27];

        /* subtract 351 * a(i-26) if i ≥ 26 */
        if (i >= 26)
            sum -= C351 * buf[(i - 26) % 27];

        buf[i % 27] = sum;                /* store a(i) */
    }

    return buf[N % 27];
}
