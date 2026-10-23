/*
 * A041276  Numerators of continued fraction convergents to sqrt(151).
 * https://oeis.org/A041276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(151). * * Let
 *            p_{-2} = 0, p_{-1} = 1. * For k ≥ 0 let a_0 = 12 and for k ≥ 1 *
 *            a_k = 3,2,7,1,3,4,1,1,1,11,1,1,1,4,3,1,7,2,3,24 repeated
 *            periodically. * The recurrence for the numerators is * p_k = a_k
 *            * p_{k-1} + p_{k-2}. * Then a(n) = p_n for n ≥ 0
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction (excluding a_0). */
    static const seq_t period[20] = {
        (seq_t)3,  (seq_t)2,  (seq_t)7,  (seq_t)1,  (seq_t)3,
        (seq_t)4,  (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)11,
        (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)4,  (seq_t)3,
        (seq_t)1,  (seq_t)7,  (seq_t)2,  (seq_t)3,  (seq_t)24
    };

    seq_t p_prev2 = (seq_t)0;   /* p_{-2} */
    seq_t p_prev1 = (seq_t)1;   /* p_{-1} */
    seq_t p_curr  = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = (seq_t)12;                         /* a_0 */
        } else {
            /* a_k for k ≥ 1 repeats the period above. */
            a_i = period[(i - 1) % 20];
        }
        p_curr = a_i * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
    return p_curr;
}
