/*
 * A170312  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A170312: number of reduced words of length n in a Coxeter group on 15 generators.
   Recurrence derived from the rational generating function:
   a(0) = 1
   For 1 ≤ n ≤ 41 :   a(n) = 2 + 13·∑_{k=1}^{n} a(n–k)
   For n ≥ 42   :   a(n) = 1 + 13·∑_{k=1}^{41} a(n–k) − 91·a(n–42)
   (The constants come from the numerator and denominator of the gf.)
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for computed values and also for the recurrence constants */
    static seq_t *seq    = NULL;   /* array of a(n) */
    static size_t cap    = 0;      /* current capacity of seq */
    static seq_t b[43];            /* numerator coefficients: b[0]=1, b[1..41]=2, b[42]=1 */
    static int init = 0;           /* flag whether b has been initialized */

    if (!init) {
        b[0] = (seq_t)1;
        for (idx_t i = 1; i <= 41; ++i)
            b[i] = (seq_t)2;
        b[42] = (seq_t)1;
        init = 1;
    }

    if (n < cap)
        return seq[n];

    size_t new_cap = (size_t)n + 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));

    for (idx_t i = (idx_t)cap; i <= n; ++i) {
        seq_t sum41 = (seq_t)0;
        idx_t limit = i < 41 ? i : 41;          /* at most 41 terms */
        for (idx_t k = 1; k <= limit; ++k) {
            sum41 += (seq_t)13 * seq[i - k];
        }

        seq_t val = b[i];                     /* base term b[i] (zero for i>42) */
        val += sum41;

        if (i >= 42) {
            seq_t term42 = (seq_t)91 * seq[i - 42];
            val -= term42;
        }
        seq[i] = val;
    }

    cap = new_cap;
    return seq[n];
}
