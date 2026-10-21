/*
 * A070052  Number of Bottleneck-Monge matrices with 4 rows.
 * https://oeis.org/A070052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 98*a(n-2) + 364*a(n-3) - 840*a(n-4)
 *            +1232*a(n-5) -1120*a(n-6) + 576*a(n-7) -128*a(n-8) (recurrence
 *            of A070052 for n ≥ 9, with initial terms below)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 43, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* First eight terms (offset 1) */
    static const seq_t init[8] = {
        16,        /* a(1) */
        87,        /* a(2) */
        384,       /* a(3) */
        1516,      /* a(4) */
        5535,      /* a(5) */
        19030,     /* a(6) */
        62347,     /* a(7) */
        196301     /* a(8) */
    };

    if (n <= 8) {
        /* n is 1-based; array is 0-based */
        return init[n - 1];
    }

    /* Use 64‑bit (or larger) local variables to hold the last eight terms */
    seq_t a1 = init[0], a2 = init[1], a3 = init[2], a4 = init[3];
    seq_t a5 = init[4], a6 = init[5], a7 = init[6], a8 = init[7];

    for (idx_t k = 9; k <= n; ++k) {
        /* Compute a(k) as the difference of two positive sums
           to avoid negative intermediate values.          */
        seq_t pos = 15 * a8 + 364 * a6 + 1232 * a4 + 576 * a2;
        seq_t neg = 98 * a7 + 840 * a5 + 1120 * a3 + 128 * a1;
        seq_t an   = pos - neg;

        /* Shift the window of last eight terms */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = a8;
        a8 = an;
    }

    return a8;
}
