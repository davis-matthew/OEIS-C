/*
 * A207148  Number of (n+1) X 8 0..2 arrays with every 2 X 2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A207148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 48, u64 n <= 112, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A207148
 *  Recurrence (valid for n ≥ 15):
 *       a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3)
 *  Initial values:
 *       a(1)=315, a(2)=218, a(3)=619, a(4)=434,
 *       a(5)=1238, a(6)=866, a(7)=2482, a(8)=1730,
 *       a(9)=4946, a(10)=3458, a(11)=9874, a(12)=6914,
 *       a(13)=19730, a(14)=13826
 *
 *  The implementation stores the first 14 terms in a static array
 *  and iterates the recurrence for n > 14 using only three
 *  successive values.  All arithmetic is performed in seq_t,
 *  so overflow behaviour follows the usual unsigned integer rules.
 */

#include <stddef.h>

/* Initial values up to n = 14.  Index 0 is unused. */
static const seq_t seeds[15] = {
    (seq_t)0,    /* dummy for index 0 */
    (seq_t)315,  /* a(1) */
    (seq_t)218,  /* a(2) */
    (seq_t)619,  /* a(3) */
    (seq_t)434,  /* a(4) */
    (seq_t)1238, /* a(5) */
    (seq_t)866,  /* a(6) */
    (seq_t)2482, /* a(7) */
    (seq_t)1730, /* a(8) */
    (seq_t)4946, /* a(9) */
    (seq_t)3458, /* a(10) */
    (seq_t)9874, /* a(11) */
    (seq_t)6914, /* a(12) */
    (seq_t)19730,/* a(13) */
    (seq_t)13826 /* a(14) */
};

/* External linkage function required by the build. */
seq_t a(idx_t n)
{
    /* Return the initial values for n ≤ 14. */
    if (n <= 14)
        return seeds[n];

    /* Use the last three terms as a sliding window. */
    seq_t a_nm1 = seeds[14];  /* a(n-1) */
    seq_t a_nm2 = seeds[13];  /* a(n-2) */
    seq_t a_nm3 = seeds[12];  /* a(n-3) */
    seq_t a_n   = a_nm1;      /* placeholder for a(n) */

    /* Iterate from i = 15 up to n. */
    for (idx_t i = 15; i <= n; ++i)
    {
        /* a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) */
        a_n = a_nm1 + (a_nm2 + a_nm2) - (a_nm3 + a_nm3);

        /* Slide the window for the next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
