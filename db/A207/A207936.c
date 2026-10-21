/*
 * A207936  Number of n X 6 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) * +
 *            21*a(n-5) - 7*a(n-6) + a(n-7) * (Recurrence) with initial values
 *            * a(1) = 22, a(2) = 484, a(3) = 2706, a(4) = 9430, * a(5) =
 *            25490, a(6) = 58602, a(7) = 120276
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 51, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 7 terms of the sequence */
    static const seq_t init[7] = {
        22ULL,   /* a(1) */
        484ULL,  /* a(2) */
        2706ULL, /* a(3) */
        9430ULL, /* a(4) */
        25490ULL,/* a(5) */
        58602ULL,/* a(6) */
        120276ULL/* a(7) */
    };

    /* For the first 7 indices, just return the stored value */
    if (n <= 7)
        return init[n - 1];

    /* Use a sliding window of the last 7 computed values */
    seq_t prev[7];
    for (idx_t i = 0; i < 7; ++i)
        prev[i] = init[i];

    seq_t cur = 0;

    /* Compute terms from 8 up to n */
    for (idx_t k = 8; k <= n; ++k) {
        cur = 7 * prev[6]
            - 21 * prev[5]
            + 35 * prev[4]
            - 35 * prev[3]
            + 21 * prev[2]
            - 7 * prev[1]
            + prev[0];

        /* Shift the window: drop the oldest term and add the newest */
        for (idx_t t = 0; t < 6; ++t)
            prev[t] = prev[t + 1];
        prev[6] = cur;
    }

    /* cur holds a(n), and prev[6] is equal to cur */
    return cur;
}
