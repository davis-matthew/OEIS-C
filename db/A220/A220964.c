/*
 * A220964  Equals one maps: number of nX2 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          king-move neighbors in a random 0..2 nX2 array.
 * https://oeis.org/A220964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -3*a(n-2) -22*a(n-3) +15*a(n-4) -36*a(n-5)
 *            +36*a(n-6) -16*a(n-7) +4*a(n-8) -8*a(n-9) -40*a(n-10)
 *            +32*a(n-11) +4*a(n-12) -16*a(n-13)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial terms a(1) .. a(13) */
    static const seq_t init[13] = {
        (seq_t)2,     /* a(1) */
        (seq_t)8,     /* a(2) */
        (seq_t)40,    /* a(3) */
        (seq_t)176,   /* a(4) */
        (seq_t)744,   /* a(5) */
        (seq_t)3144,  /* a(6) */
        (seq_t)13128, /* a(7) */
        (seq_t)54424, /* a(8) */
        (seq_t)224120,/* a(9) */
        (seq_t)918688,/* a(10) */
        (seq_t)3750192,/* a(11) */
        (seq_t)15259608,/* a(12) */
        (seq_t)61920992 /* a(13) */
    };

    if (n <= 13)
        return init[n - 1];

    /* Sliding window of the last 13 values, newest first:
       last[0] = a(k-1), last[1] = a(k-2), …, last[12] = a(k-13)   */
    seq_t last[13];
    for (int i = 0; i < 13; ++i)
        last[i] = init[12 - i];

    seq_t res = 0;
    for (idx_t k = 14; k <= n; ++k) {
        seq_t sum1 = 0;   /* positive contributions */
        sum1 += (seq_t)6  * last[0];  /* a(k-1) */
        sum1 += (seq_t)15 * last[3];  /* a(k-4) */
        sum1 += (seq_t)36 * last[5];  /* a(k-6) */
        sum1 += (seq_t)4  * last[7];  /* a(k-8) */
        sum1 += (seq_t)32 * last[10]; /* a(k-11) */
        sum1 += (seq_t)4  * last[11]; /* a(k-12) */

        seq_t sum2 = 0;   /* negative contributions */
        sum2 += (seq_t)3  * last[1];  /* a(k-2) */
        sum2 += (seq_t)22 * last[2];  /* a(k-3) */
        sum2 += (seq_t)36 * last[4];  /* a(k-5) */
        sum2 += (seq_t)16 * last[6];  /* a(k-7) */
        sum2 += (seq_t)8  * last[8];  /* a(k-9) */
        sum2 += (seq_t)40 * last[9];  /* a(k-10) */
        sum2 += (seq_t)16 * last[12]; /* a(k-13) */

        res = sum1 - sum2;  /* a(k) */

        /* Shift the window for the next iteration */
        for (int i = 12; i > 0; --i)
            last[i] = last[i - 1];
        last[0] = res;
    }
    return res;
}
