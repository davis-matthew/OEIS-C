/*
 * A200185  Number of -n..n arrays x(0..6) of 7 elements with zero sum and no
 *          two consecutive declines, no adjacent equal elements, and no
 *          element more than one greater than the previous (random base
 *          sawtooth pattern).
 * https://oeis.org/A200185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -a(n-1)+a(n-3)+2*a(n-4)+2*a(n-5)+2*a(n-6)
 *            -2*a(n-8)-3*a(n-9)-3*a(n-10)-2*a(n-11)
 *            +2*a(n-13)+2*a(n-14)+2*a(n-15)+a(n-16) -a(n-18)-a(n-19) for n >
 *            21 Initial values for 1≤n≤21: 10, 29, 78, 100, 186, 246, 380,
 *            464, 686, 798, 1096, 1276, 1658, 1878, 2408, 2668, 3306, 3672,
 *            4432, 4852, 5814
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but safe */

    /* Initial values for n = 1..21 */
    static const seq_t init[21] = {
        10, 29, 78, 100, 186, 246, 380, 464, 686, 798,
        1096, 1276, 1658, 1878, 2408, 2668, 3306, 3672, 4432,
        4852, 5814
    };

    if (n <= 21)
        return init[n - 1];

    /* Buffer of the last 19 terms: prev[0] = a(k-1),
       prev[1] = a(k-2), ..., prev[18] = a(k-19), where k is the
       current index we are computing. */
    seq_t prev[19];
    for (size_t i = 0; i < 19; ++i)
        prev[i] = init[20 - i]; /* a(21) down to a(3) */

    seq_t next = 0;

    for (idx_t cur = 22; cur <= n; ++cur) {
        /* Positive contributions */
        seq_t pos = prev[2];              /* a(cur-3) */
        pos += 2 * prev[3];               /* 2*a(cur-4) */
        pos += 2 * prev[4];               /* 2*a(cur-5) */
        pos += 2 * prev[5];               /* 2*a(cur-6) */
        pos += 2 * prev[12];              /* 2*a(cur-13) */
        pos += 2 * prev[13];              /* 2*a(cur-14) */
        pos += 2 * prev[14];              /* 2*a(cur-15) */
        pos += prev[15];                  /* a(cur-16) */

        /* Negative contributions */
        seq_t neg = prev[0];              /* a(cur-1) */
        neg += 2 * prev[7];               /* 2*a(cur-8) */
        neg += 3 * prev[8];               /* 3*a(cur-9) */
        neg += 3 * prev[9];               /* 3*a(cur-10) */
        neg += 2 * prev[10];              /* 2*a(cur-11) */
        neg += prev[17];                  /* a(cur-18) */
        neg += prev[18];                  /* a(cur-19) */

        next = pos - neg;

        /* Shift the ring buffer to drop a(cur-19) and add a(cur) */
        for (size_t i = 18; i > 0; --i)
            prev[i] = prev[i - 1];
        prev[0] = next;
    }

    return next;
}
