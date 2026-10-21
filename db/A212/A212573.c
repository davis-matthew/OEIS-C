/*
 * A212573  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          |w-x|>|x-y|+|y-z|.
 * https://oeis.org/A212573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 379, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - a(n-2) - 5*a(n-3) + 5*a(n-4) + a(n-5)
 *              - 3*a(n-6) + a(n-7)      (for n ≥ 8)
 * Initial terms a(0)…a(7) = 0,0,2,10,36,92,202,386.
 * This implementation uses an O(n) iterative approach with
 * constant auxiliary space.  It works for any build where
 * seq_t can represent the intermediate results. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base values for 0 ≤ n ≤ 7 */
    static const seq_t init[8] = {0, 0, 2, 10, 36, 92, 202, 386};

    if (n < 8) {
        return init[n];
    }

    /* buffer holds a(n-7), …, a(n-1) */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i) {
        buf[i] = init[i + 1];  /* buf[0] = a(1), ..., buf[6] = a(7) */
    }

    for (idx_t cur = 8; cur <= n; ++cur) {
        /* next = 3*a(n-1) - a(n-2) - 5*a(n-3) + 5*a(n-4)
         *      + a(n-5) - 3*a(n-6) + a(n-7)
         *
         * To avoid underflow we group positive and negative terms:
         */
        seq_t pos = 3 * buf[6] + 5 * buf[3] + buf[2] + buf[0];
        seq_t neg = buf[5] + 5 * buf[4] + 3 * buf[1];
        seq_t next = pos - neg;

        /* shift buffer left: drop a(n-7), keep a(n-6)…a(n-1) */
        for (idx_t i = 0; i < 6; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[6] = next;          /* now buf[6] = a(cur) */
    }

    return buf[6];
}
