/*
 * A224605  Number of (n+2) X 9 0..2 matrices with each 3 X 3 subblock
 *          idempotent.
 * https://oeis.org/A224605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) -
 *            a(n-6) for n > 8. Initial values: a(1) = 1694, a(2) = 1221, a(3)
 *            = 1455, a(4) = 1657, a(5) = 2091, a(6) = 2656, a(7) = 3308, a(8)
 *            = 4272
 * Range:     u8 none, u16 n <= 16, u32 n <= 45, u64 n <= 103, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Seed values for indices 1 .. 8.  Index 0 is unused. */
static const seq_t _seed[9] = {
    (seq_t)0,  /* 0 */
    (seq_t)1694,   /* 1 */
    (seq_t)1221,   /* 2 */
    (seq_t)1455,   /* 3 */
    (seq_t)1657,   /* 4 */
    (seq_t)2091,   /* 5 */
    (seq_t)2656,   /* 6 */
    (seq_t)3308,   /* 7 */
    (seq_t)4272    /* 8 */
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is defined for n >= 1 */
    if (n <= 8) return _seed[n];

    /* Initialise the last six values needed for the recurrence. */
    seq_t a_nm1 = _seed[8];   /* a(n-1) */
    seq_t a_nm2 = _seed[7];   /* a(n-2) */
    seq_t a_nm3 = _seed[6];   /* a(n-3) */
    seq_t a_nm4 = _seed[5];   /* a(n-4) */
    seq_t a_nm5 = _seed[4];   /* a(n-5) */
    seq_t a_nm6 = _seed[3];   /* a(n-6) */

    seq_t cur = 0;
    for (idx_t k = 9; k <= n; ++k) {
        /* Compute the next term using the recurrence.
           Expressed as (sum of positive terms) - (sum of negative terms)
           to avoid unsigned underflow when seq_t is an unsigned type. */
        seq_t sum1 = 4 * a_nm1 + 5 * a_nm3 + 3 * a_nm5;
        seq_t sum2 = 6 * a_nm2 + 4 * a_nm4 + a_nm6;
        cur = sum1 - sum2;

        /* Shift the window of the last six terms. */
        a_nm6 = a_nm5;
        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}
