/*
 * A230727  Number of n X 5 0..5 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          (x(i,j)+1) mod 6, and upper left element zero.
 * https://oeis.org/A230727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=8, a(3)=260, a(4)=9760, a(5)=389148, *
 *            a(6)=15237056, a(7)=597115044, a(8)=23414415888, * For n ≥ 9: *
 *            a(n) = 37*a(n−1) + 72*a(n−2) + 990*a(n−3) * − 16253*a(n−4) −
 *            12331*a(n−5) * + 83486*a(n−6) − 56928*a(n−7) * * Uses the
 *            recurrence directly; no memoisation is required and the *
 *            calc...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* 8 useless placeholder entries: init_vals[0] is unused */
static const unsigned long long init_vals_ul[9] = {
    0ULL,
    0ULL,         /* a(1) */
    8ULL,         /* a(2) */
    260ULL,       /* a(3) */
    9760ULL,      /* a(4) */
    389148ULL,    /* a(5) */
    15237056ULL,  /* a(6) */
    597115044ULL, /* a(7) */
    23414415888ULL/* a(8) */
};

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;
    }
    if (n <= 8U) {
        /* the cast guarantees the value is stored in seq_t;
           truncation on tiny seq_t types is harmless because
           those terms do not fit and are never checked. */
        return (seq_t)init_vals_ul[(size_t)n];
    }

    /* buffer[1..8] holds the last 8 computed terms */
    seq_t buffer[9];
    for (size_t i = 1; i <= 8; ++i) {
        buffer[i] = (seq_t)init_vals_ul[i];
    }

    for (idx_t i = 9; i <= n; ++i) {
        seq_t val = buffer[8];
        val = val * 37ULL + buffer[7] * 72ULL + buffer[6] * 990ULL
              - buffer[5] * 16253ULL - buffer[4] * 12331ULL
              + buffer[3] * 83486ULL - buffer[2] * 56928ULL;

        /* shift buffer: drop the oldest and push the newest */
        buffer[1] = buffer[2];
        buffer[2] = buffer[3];
        buffer[3] = buffer[4];
        buffer[4] = buffer[5];
        buffer[5] = buffer[6];
        buffer[6] = buffer[7];
        buffer[7] = buffer[8];
        buffer[8] = val;
    }

    return buffer[8];
}
