/*
 * A204468  Number of 4-element subsets that can be chosen from {1,2,...,4*n}
 *          having element sum 8*n+2.
 * https://oeis.org/A204468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204468
 * Linear recurrence derived from the generating function:
 *   a(0)=0, a(1)=1, a(2)=8, a(3)=33, a(4)=86, a(5)=177
 *   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-5) - a(n-6)   (n ≥ 6) */
#include <stddef.h>

static const seq_t init[6] = {
    (seq_t)0, (seq_t)1, (seq_t)8, (seq_t)33, (seq_t)86, (seq_t)177
};

seq_t a(idx_t n)
{
    if (n < 6)          /* small indices */
        return init[(size_t)n];

    /* keep the last six computed values */
    seq_t v[6];
    for (size_t i = 0; i < 6; ++i)
        v[i] = init[i];          /* v[0] = a(0), … , v[5] = a(5) */

    seq_t next;
    for (idx_t k = 6; k <= n; ++k)
    {
        /* recurrence:
           a(k) = 3*a(k-1) - 3*a(k-2) + 2*a(k-3) - 3*a(k-4) + 3*a(k-5) - a(k-6) */
        next = (seq_t)3 * v[5]
               - (seq_t)3 * v[4]
               + (seq_t)2 * v[3]
               - (seq_t)3 * v[2]
               + (seq_t)3 * v[1]
               - v[0];

        /* roll the window: discard a(k-6), shift others, insert a(k) */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }

    return v[5];   /* this is a(n) */
}
