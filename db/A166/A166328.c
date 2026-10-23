/*
 * A166328  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 2*a(n-1) + 2*a(n-2)
 *            + ... + 2*a(n-10) - 3*a(n-11) + c(n) where c(0)=1, c(1..10)=2,
 *            c(11)=1, c(n)=0 for n≥12. Initial values: a(0)=1, a(1)=4,
 *            a(2)=12, a(3)=36, a(4)=108, a(5)=324, a(6)=972, a(7)=2916,
 *            a(8)=8748, a(9)=26244, a(10)=78732, a(11)=236190
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t base[12] = {
    (seq_t)1,  (seq_t)4,  (seq_t)12, (seq_t)36, (seq_t)108, (seq_t)324,
    (seq_t)972, (seq_t)2916, (seq_t)8748, (seq_t)26244, (seq_t)78732,
    (seq_t)236190
};

seq_t a(idx_t n)
{
    if (n < 12)
        return base[n];

    seq_t prev[11];
    /* Initialize prev[0..10] with a(1) .. a(11) */
    for (idx_t i = 0; i < 11; ++i)
        prev[i] = base[i + 1];

    seq_t result = 0;
    for (idx_t i = 12; i <= n; ++i)
    {
        seq_t sum10 = 0;
        for (idx_t k = 1; k < 11; ++k)
            sum10 += prev[k];
        result = 2 * sum10 - 3 * prev[0];   /* c(i) = 0 for i ≥ 12 */

        /* Shift window: drop prev[0], shift prev[1..10] left, insert result */
        for (idx_t k = 0; k < 10; ++k)
            prev[k] = prev[k + 1];
        prev[10] = result;
    }
    return result;
}
