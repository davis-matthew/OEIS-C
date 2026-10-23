/*
 * A170325  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) =
 *            26*a(n-1)+26*a(n-2)+...+26*a(n-41)-351*a(n-42) for n ≥ 42, with
 *            initial values obtained from the generating function: a(0)=1,
 *            a(1)=28, a(2)=756, a(3)=20412, a(4)=551124, a(5)=14880348,
 *            a(6)=401769396, a(7)=10847773692, a(8)=292889889684,
 *            a(9)=7908027021...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* number of terms to compute */
    size_t len = (size_t)n + 1;

    /* allocate array of seq_t, initialized to zero */
    seq_t *coef = alloc_seq(len);

    /* a(0) = 1 */
    coef[0] = (seq_t)1;

    for (size_t k = 1; k < len; ++k)
    {
        /* numerator coefficient: 1 at n=0 and n=42, 2 otherwise up to 41 */
        seq_t num;
        if (k == 0 || k == 42)
            num = (seq_t)1;
        else if (k <= 41)
            num = (seq_t)2;
        else
            num = (seq_t)0;           /* will never be used for k>42 */

        /* sum of the last up to 41 previous coefficients */
        seq_t sum = (seq_t)0;
        size_t up = k < 41 ? k : 41;
        for (size_t i = 1; i <= up; ++i)
            sum += coef[k - i];

        /* apply recurrence */
        seq_t term = num + (seq_t)26 * sum;
        if (k >= 42)
            term -= (seq_t)351 * coef[k - 42];

        coef[k] = term;
    }

    seq_t result = coef[n];
    free_seq(coef);
    return result;
}
