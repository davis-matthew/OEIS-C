/*
 * A041496  Numerators of continued fraction convergents to sqrt(265).
 * https://oeis.org/A041496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(265) Convergents
 *            are produced by the recurrence: h(-2) = 0, h(-1) = 1 p(-2) = 1,
 *            p(-1) = 0 for i >= 0: a_i = continued fraction coefficient at
 *            position i h_i = a_i * h_{i-1} + h_{i-2} p_i = a_i * p_{i-1} +
 *            p_{i-2} where a_0 = floor(sqrt(265)) = 16 a...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Continued fraction coefficients:
       a_0 = 16, a_1..a_8 = {3,1,1,2,2,1,1,3}, a_9 = 32, then repeat */
    static const unsigned long long COEF[9] = {
        3ULL, 1ULL, 1ULL, 2ULL, 2ULL, 1ULL, 1ULL, 3ULL, 32ULL
    };
    const seq_t a0 = (seq_t)16;

    /* Initial previous numerators and denominators (h_{-2}, h_{-1}, p_{-2}, p_{-1}) */
    seq_t h_prev_prev = (seq_t)0;
    seq_t h_prev      = (seq_t)1;
    seq_t p_prev_prev = (seq_t)1;
    seq_t p_prev      = (seq_t)0;

    seq_t h_curr = 0, p_curr = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai;
        if (i == 0) {
            ai = a0;
        } else {
            ai = (seq_t)COEF[(i - 1) % 9];
        }

        /* h_i = a_i * h_{i-1} + h_{i-2} */
        h_curr = ai * h_prev + h_prev_prev;
        /* p_i = a_i * p_{i-1} + p_{i-2} */
        p_curr = ai * p_prev + p_prev_prev;

        h_prev_prev = h_prev;
        h_prev = h_curr;
        p_prev_prev = p_prev;
        p_prev = p_curr;
    }

    return h_curr;
}
