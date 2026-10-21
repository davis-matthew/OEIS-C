/*
 * A217880  Number of n element 0..2 arrays with each element the minimum of 5
 *          adjacent elements of a random 0..2 array of n+4 elements.
 * https://oeis.org/A217880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 72, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A217880
   Empirical recurrence:
   a(1)=3, a(2)=9, a(3)=22, a(4)=46, a(5)=86, a(6)=148,
   a(7)=244, a(8)=402, a(9)=685, a(10)=1223, a(11)=2263
   For n≥12:
   a(n) = 3*a(n‑1) – 3*a(n‑2) + a(n‑3) + 3*a(n‑6) – a(n‑7)
          + a(n‑8) + a(n‑9) + a(n‑10) + a(n‑11)
   Indexed with a(1) as the first term.                */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* Sequence values for indices 0..11; a[0] is unused. */
    static const seq_t init[12] = {
        0,  /* unused index 0 */
        3,  /* a(1) */
        9,  /* a(2) */
        22, /* a(3) */
        46, /* a(4) */
        86, /* a(5) */
        148,/* a(6) */
        244,/* a(7) */
        402,/* a(8) */
        685,/* a(9) */
        1223,/* a(10) */
        2263 /* a(11) */
    };

    /* For n ≤ 11, return the pre‑computed term. */
    if (n <= 11) {
        /* n is ≥1 by the contract of the function. */
        return init[(idx_t)n];
    }

    /* Circular buffer of 12 slots to hold the last 11 values plus a
       placeholder for the current value.  Index 0 is unused for
       simplicity; we keep indices modulo 12 for the recurrence. */
    seq_t buf[12];
    for (idx_t i = 0; i < 12; ++i) {
        buf[i] = init[i];
    }

    /* Compute terms 12 .. n, one at a time, updating the ring buffer. */
    for (idx_t i = 12; i <= n; ++i) {
        idx_t cur  = i % 12;
        idx_t im1  = (i - 1) % 12;
        idx_t im2  = (i - 2) % 12;
        idx_t im3  = (i - 3) % 12;
        idx_t im6  = (i - 6) % 12;
        idx_t im7  = (i - 7) % 12;
        idx_t im8  = (i - 8) % 12;
        idx_t im9  = (i - 9) % 12;
        idx_t im10 = (i -10) % 12;
        idx_t im11 = (i -11) % 12;

        /* Evaluate the recurrence with intermediate additions and
           subtractions arranged to avoid unsigned underflow. */
        seq_t val = buf[im1];
        val += buf[im1];
        val += buf[im1];

        val -= buf[im2];
        val -= buf[im2];
        val -= buf[im2];

        val += buf[im3];

        val += buf[im6];
        val += buf[im6];
        val += buf[im6];

        val -= buf[im7];

        val += buf[im8];
        val += buf[im9];
        val += buf[im10];
        val += buf[im11];

        buf[cur] = val;
    }

    return buf[n % 12];
}
