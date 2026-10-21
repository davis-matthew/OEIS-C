/*
 * A212587  Walks of length n on the x-axis using steps {1,0,-1} and visiting
 *          no point more than twice.
 * https://oeis.org/A212587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(0)=1, a(1)=4, a(2)=12, a(3)=30, a(4)=66, a(5)=148, a(6)=316,
 *            a(7)=682, a(8)=1438, and for n≥9 a(n) =
 *            2*a(n-1)+2*a(n-2)-4*a(n-3)+2*a(n-5)-2*a(n-6)+a(n-8). This
 *            recurrence follows from the generating function of the sequence
 *            and holds for all n≥9
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const seq_t BASE[9] = {
    (seq_t)1,   /* a(0) */
    (seq_t)4,   /* a(1) */
    (seq_t)12,  /* a(2) */
    (seq_t)30,  /* a(3) */
    (seq_t)66,  /* a(4) */
    (seq_t)148, /* a(5) */
    (seq_t)316, /* a(6) */
    (seq_t)682, /* a(7) */
    (seq_t)1438 /* a(8) */
};

seq_t a(idx_t n) {
    /* Return small values directly */
    if (n < 9) return BASE[n];

    /* Circular buffer for the last 9 computed terms */
    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i) buf[i] = BASE[i];

    seq_t next = 0;
    for (idx_t i = 9; i <= n; ++i) {
        /* indices of the required previous terms */
        idx_t i1 = (i - 1) % 9;
        idx_t i2 = (i - 2) % 9;
        idx_t i3 = (i - 3) % 9;
        idx_t i5 = (i - 5) % 9;
        idx_t i6 = (i - 6) % 9;
        idx_t i8 = (i - 8) % 9;

        /* Compute next value using the recurrence */
        seq_t t1 = (seq_t)2 * buf[i1];
        seq_t t2 = (seq_t)2 * buf[i2];
        seq_t t3 = (seq_t)4 * buf[i3];
        seq_t t4 = (seq_t)2 * buf[i5];
        seq_t t5 = (seq_t)2 * buf[i6];
        seq_t t6 = buf[i8];

        next = t1 + t2;
        next -= t3;
        next += t4;
        next -= t5;
        next += t6;

        buf[i % 9] = next;
    }
    return next;
}
