/*
 * A128439  a(n) = floor(n*t^n), where t=golden ratio=(1+sqrt(5))/2.
 * https://oeis.org/A128439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 4*a(n-3) - 2*a(n-4) + 2*a(n-5) +
 *            a(n-6) (n >= 7). Initial values: a(1)=1, a(2)=5, a(3)=12,
 *            a(4)=27, a(5)=55, a(6)=107
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial terms (offset 1) */
    static const seq_t init[6] = { (seq_t)1, (seq_t)5, (seq_t)12,
                                   (seq_t)27, (seq_t)55, (seq_t)107 };
    if (n <= 6)
        return init[n-1];

    /* Circular buffer holding the last 6 values: buf[0]=a(n-6), buf[5]=a(n-1) */
    seq_t buf[6];
    for (int i = 0; i < 6; ++i)
        buf[i] = init[i];

    /* Compute up to n */
    for (idx_t k = 7; k <= n; ++k)
    {
        /* Extract previous terms */
        seq_t a1 = buf[0]; /* a(n-6) */
        seq_t a2 = buf[1]; /* a(n-5) */
        seq_t a3 = buf[2]; /* a(n-4) */
        seq_t a4 = buf[3]; /* a(n-3) */
        seq_t a5 = buf[4]; /* a(n-2) */
        seq_t a6 = buf[5]; /* a(n-1) */

        /* Compute the recurrence using only additions and subtractions */
        seq_t term1 = a6 + a6;            /* 2*a(n-1) */
        seq_t term2 = a5 + a5;            /* 2*a(n-2) */
        seq_t term3 = a2 + a2;            /* 2*a(n-5) */
        seq_t sum_pos = term1 + term2 + term3 + a1; /* a(n-6) added once */

        /* Subtract 4*a(n-3) and 2*a(n-4) */
        seq_t sub1 = a4 + a4 + a4 + a4;   /* 4*a(n-3) */
        seq_t sub2 = a3 + a3;             /* 2*a(n-4) */
        seq_t new_val = sum_pos - (sub1 + sub2);

        /* Shift buffer: drop oldest, push new_val */
        for (int i = 0; i < 5; ++i)
            buf[i] = buf[i+1];
        buf[5] = new_val;
    }

    /* After loop, buf[5] holds a(n) */
    return buf[5];
}
