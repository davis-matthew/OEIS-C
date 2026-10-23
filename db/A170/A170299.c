/*
 * A170299  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for 1<=n<=40: a(n)=2+48*sum_{k=0}^{n-1} a(k); 
   For n>=41: a(n)=48*a(n-1)+48*a(n-2)+...+48*a(n-40)-1176*a(n-41). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static table for the first 42 terms (0..41) */
    static seq_t init_terms[42];
    static bool init_done = false;

    if (!init_done) {
        init_done = true;
        /* compute terms 0..40 */
        seq_t running_sum = 0;
        for (idx_t i = 0; i <= 40; ++i) {
            if (i == 0) {
                init_terms[0] = 1;
                running_sum = init_terms[0];
            } else {
                init_terms[i] = 2 + (seq_t)48 * running_sum;
                running_sum += init_terms[i];
            }
        }
        /* compute term 41 */
        seq_t sum48 = 0;
        for (idx_t i = 1; i <= 40; ++i)
            sum48 += (seq_t)48 * init_terms[i];
        init_terms[41] = 1 + sum48 - (seq_t)1176 * init_terms[0];
    }

    /* return early for n <= 41 */
    if (n <= 41)
        return init_terms[n];

    /* ring buffer holding the last 41 terms for the recurrence */
    seq_t buffer[41];
    for (idx_t i = 1; i <= 41; ++i)
        buffer[i - 1] = init_terms[i];   /* buffer[0] = a(1), ..., buffer[40] = a(41) */

    for (idx_t i = 42; i <= n; ++i) {
        seq_t sum48 = 0;
        for (idx_t k = 1; k < 41; ++k)   /* k=1..40, use buffer[1]..buffer[40] */
            sum48 += (seq_t)48 * buffer[k];

        seq_t sub = (seq_t)1176 * buffer[0];    /* buffer[0] = a(i-41) */
        seq_t new_term = sum48 - sub;

        /* shift left and add the new term at the end */
        for (idx_t k = 0; k < 40; ++k)
            buffer[k] = buffer[k + 1];
        buffer[40] = new_term;
    }

    return buffer[40];   /* after loop, buffer[40] is a(n) */
}
