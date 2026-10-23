/*
 * A167897  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167897: number of reduced words of length n in the Coxeter
 * group on 6 generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 *
 * Recurrence:  a(0)=1, a(1)=6, …, a(15)=36621093750.
 * For n = 16:   a(16)=4*(a(15)+…+a(1)) -10*a(0) + 1
 * For n ≥ 17:   a(n)=4*(a(n-1)+…+a(n-15)) -10*a(n-16)            */
/* Note: the generating function is
 *        (t^16+2*t^15+…+2*t+1)/(10*t^16-4*t^15-…-4*t+1)        */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Initial 16 terms (a(0) … a(15)) */
    static const seq_t init[16] = {
        (seq_t)1ULL,
        (seq_t)6ULL,
        (seq_t)30ULL,
        (seq_t)150ULL,
        (seq_t)750ULL,
        (seq_t)3750ULL,
        (seq_t)18750ULL,
        (seq_t)93750ULL,
        (seq_t)468750ULL,
        (seq_t)2343750ULL,
        (seq_t)11718750ULL,
        (seq_t)58593750ULL,
        (seq_t)292968750ULL,
        (seq_t)1464843750ULL,
        (seq_t)7324218750ULL,
        (seq_t)36621093750ULL
    };

    if (n < 16)                         /* return a precomputed value */
        return init[(size_t)n];

    /* Sliding window of the last 16 values:
     * window[0] = a(n-16), window[15] = a(n-1)  */
    seq_t window[16];
    for (int i = 0; i < 16; ++i)
        window[i] = init[i];

    seq_t result = 0;
    for (idx_t i = 16; i <= n; ++i)
    {
        seq_t sum = 0;
        /* 4 * sum of the 15 newest terms */
        for (int k = 1; k < 16; ++k) sum += window[k];
        sum = 4 * sum - 10 * window[0];

        /* Special constant term for n == 16 */
        if (i == 16) sum += (seq_t)1;

        result = sum;                 /* a(i) */

        /* Shift window left and append the new term */
        for (int k = 0; k < 15; ++k)
            window[k] = window[k + 1];
        window[15] = sum;
    }

    return result;
}
