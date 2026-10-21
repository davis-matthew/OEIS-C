/*
 * A219596  Number of 3 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..2 3 X n array.
 * https://oeis.org/A219596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) +
 *            252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) -
 *            a(n-10); Initial values: a(1)=6, a(2)=21, a(3)=84, a(4)=233,
 *            a(5)=550, a(6)=1188, a(7)=2415, a(8)=4684, a(9)=8746,
 *            a(10)=15833, a(11)=27945, a(12)=48286, a(13)=81907
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 46, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[13] = {
        6ULL,   /* a(1) */
        21ULL,  /* a(2) */
        84ULL,  /* a(3) */
        233ULL, /* a(4) */
        550ULL, /* a(5) */
        1188ULL,/* a(6) */
        2415ULL,/* a(7) */
        4684ULL,/* a(8) */
        8746ULL,/* a(9) */
        15833ULL,/* a(10) */
        27945ULL,/* a(11) */
        48286ULL,/* a(12) */
        81907ULL /* a(13) */
    };

    /* For n <= 13 the answer is known */
    if (n <= 13)
        return base[(size_t)n - 1];

    /* Maintain the last ten values needed for the recurrence.
       Indices: prev[0]=a(i-1), prev[1]=a(i-2), ..., prev[9]=a(i-10)   */
    seq_t prev[10];

    /* Initialise for i = 14: a(13) … a(4) */
    for (size_t k = 0; k < 10; ++k)
        prev[k] = base[12 - k];

    seq_t val = 0;
    for (idx_t i = 14; i <= n; ++i) {
        val = 10 * prev[0]
            - 45 * prev[1]
            + 120 * prev[2]
            - 210 * prev[3]
            + 252 * prev[4]
            - 210 * prev[5]
            + 120 * prev[6]
            - 45 * prev[7]
            + 10 * prev[8]
            - prev[9];

        /* Rotate the buffer: shift right and insert new value at front */
        for (size_t k = 9; k > 0; --k)
            prev[k] = prev[k-1];
        prev[0] = val;
    }

    return prev[0];
}
