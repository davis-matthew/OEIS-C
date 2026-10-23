/*
 * A166856  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*(a(n-1)+a(n-2)+…+a(n-11)) - 1176*a(n-12) + c_n * where
 *            c_n = 1 for n==12, otherwise 0. This follows from the *
 *            generating function of A166856
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 12 terms (indices 0 … 11).  They are exact integers. */
    static const seq_t init[12] = {
        1ULL,
        50ULL,
        2450ULL,
        120050ULL,
        5882450ULL,
        288240050ULL,
        14123762450ULL,
        692064360050ULL,
        33911153642450ULL,
        1661646528480050ULL,
        81420679895522450ULL,
        3989613314880600050ULL
    };

    if (n < 12)
        return init[(size_t)n];

    /* Sliding window buffer of the last 12 computed terms. */
    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* Sum of the previous 11 terms a(i-1) … a(i-11). */
    seq_t sum = 0;
    for (size_t i = 1; i < 12; ++i)
        sum += buf[i];

    seq_t val = 0;

    for (idx_t i = 12; i <= n; ++i) {
        /* Recurrence: a(i) = 48*sum - 1176*a(i-12) (+ c_i).  */
        val = (seq_t)48 * sum - (seq_t)1176 * buf[(i - 12) % 12];

        /* Add the numerator coefficient only for n = 12. */
        if (i == 12)
            ++val;

        /* Update the sliding sum for the next step:
           drop a(i-11) and add a(i). */
        sum = sum + val - buf[(i - 11) % 12];

        /* Store the new term in the buffer. */
        buf[i % 12] = val;
    }
    return val;
}
