/*
 * A224562  Number of (n+3) X 5 0..1 matrices with each 4 X 4 subblock
 *          idempotent.
 * https://oeis.org/A224562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-5) + a(n-6) - 2*a(n-7)
 *            + 2*a(n-8) + a(n-11) - a(n-12) initial values: a(1)=370,
 *            a(2)=340, a(3)=428, a(4)=517, a(5)=611, a(6)=775, a(7)=1044,
 *            a(8)=1383, a(9)=1808, a(10)=2373, a(11)=3190, a(12)=4323,
 *            a(13)=5868, a(14)=7946, a(15)=10825
 * Range:     u8 none, u16 n <= 20, u32 n <= 55, u64 n <= 123, u128 n <= 210,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Initial values a(1)..a(15) – index 0 unused */
    static const seq_t init[16] = {
        0ULL,
        370ULL,   /* a(1) */
        340ULL,   /* a(2) */
        428ULL,   /* a(3) */
        517ULL,   /* a(4) */
        611ULL,   /* a(5) */
        775ULL,   /* a(6) */
        1044ULL,  /* a(7) */
        1383ULL,  /* a(8) */
        1808ULL,  /* a(9) */
        2373ULL,  /* a(10) */
        3190ULL,  /* a(11) */
        4323ULL,  /* a(12) */
        5868ULL,  /* a(13) */
        7946ULL,  /* a(14) */
        10825ULL  /* a(15) */
    };

    /* sequence defined directly for n ≤ 15 */
    if (n <= 15)
        return init[n];

    /* coefficients of the recurrence: {offset, coefficient} */
    static const struct { int offset; int coeff; } coeffs[] = {
        {  1,  2},
        {  2, -1},
        {  3,  1},
        {  5, -2},
        {  6,  1},
        {  7, -2},
        {  8,  2},
        { 11,  1},
        { 12, -1}
    };

    /* window of the last 12 terms:
       last[0] = a(cur-12), last[1] = a(cur-11) ... last[11] = a(cur-1)   */
    seq_t last[12];

    /* For the first use (cur = 16) we need the terms a(4) .. a(15) */
    for (idx_t i = 0; i < 12; ++i)
        last[i] = init[4 + i];

    idx_t cur = 16;                       /* next term to compute */

    while (cur <= n) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;

        for (size_t i = 0; i < sizeof coeffs / sizeof coeffs[0]; ++i) {
            int  off   = coeffs[i].offset;
            int  coeff = coeffs[i].coeff;
            seq_t val   = last[12 - off];   /* a(cur - off) */

            if (coeff > 0)
                pos_sum += mul_small(val, (unsigned)coeff);
            else
                neg_sum += mul_small(val, (unsigned)(-coeff));
        }

        seq_t newval = pos_sum - neg_sum;   /* assumes pos_sum >= neg_sum */

        /* shift window right and insert new value */
        for (idx_t i = 0; i < 11; ++i)
            last[i] = last[i + 1];
        last[11] = newval;

        ++cur;
    }

    /* last[11] is a(n) after the loop */
    return last[11];
}
