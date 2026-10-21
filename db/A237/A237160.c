/*
 * A237160  Number of (n+1) X (1+1) 0..2 arrays with the maximum plus the
 *          minimum minus the upper median minus the lower median of every 2 X
 *          2 subblock equal.
 * https://oeis.org/A237160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 2*a(n-2) - 92*a(n-3) + 80*a(n-4) + 376*a(n-5)
 *            - 360*a(n-6) - 600*a(n-7) + 400*a(n-8) + 320*a(n-9) (n ≥ 10)
 *            Initial values: a(1) = 81, a(2) = 245, a(3) = 777, a(4) = 2597,
 *            a(5) = 8961, a(6) = 31837, a(7) = 115561, a(8) = 427093, a(9) =
 *            1600465
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t _a_init[10] = {
    0,        /* unused */
    81ULL,    /* a(1) */
    245ULL,   /* a(2) */
    777ULL,   /* a(3) */
    2597ULL,  /* a(4) */
    8961ULL,  /* a(5) */
    31837ULL, /* a(6) */
    115561ULL,/* a(7) */
    427093ULL,/* a(8) */
    1600465ULL/* a(9) */
};

seq_t a(idx_t n)
{
    if (n <= 9)
        return _a_init[n];

    /* Buffer for the last nine values: v[0] = a(n-9), ..., v[8] = a(n-1) */
    seq_t v[9];
    for (int i = 0; i < 9; ++i)
        v[i] = _a_init[i + 1];

    for (idx_t k = 10; k <= n; ++k) {
        seq_t newval =
            7*v[8] + 2*v[7] - 92*v[6] + 80*v[5] + 376*v[4]
            - 360*v[3] - 600*v[2] + 400*v[1] + 320*v[0];

        /* Shift the window: drop v[0], move v[1]..v[8] left, insert newval */
        for (int j = 0; j < 8; ++j)
            v[j] = v[j + 1];
        v[8] = newval;
    }

    return v[8];
}
