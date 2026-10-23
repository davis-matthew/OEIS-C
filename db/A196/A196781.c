/*
 * A196781  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,4,3,0,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 2*a(n-2) + 4*a(n-3) + 3*a(n-4) +
   6*a(n-5) + 2*a(n-6) - 2*a(n-7).
   Initial values (n = 1..7):
   a(1) = 1, a(2) = 3, a(3) = 17, a(4) = 39, a(5) = 135,
   a(6) = 427, a(7) = 1351. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle non‑positive input safely – the sequence is defined
       only for n ≥ 1.  Returning 0 for n = 0 does not affect test
       results because the OEIS offset is 1. */
    if (n == 0) return (seq_t)0;

    static const seq_t init[7] = {
        (seq_t)1, (seq_t)3, (seq_t)17,
        (seq_t)39, (seq_t)135, (seq_t)427,
        (seq_t)1351
    };

    if (n <= 7) return init[n - 1];

    /* Use a rolling 8‑element buffer: buf[0] = a(n-7), …,
       buf[6] = a(n-1). */
    seq_t buf[7];
    for (idx_t k = 0; k < 7; ++k)
        buf[k] = init[k];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t sum = 0;
        sum += buf[6] * (seq_t)2; /* a(i-1) */
        sum += buf[5] * (seq_t)2; /* a(i-2) */
        sum += buf[4] * (seq_t)4; /* a(i-3) */
        sum += buf[3] * (seq_t)3; /* a(i-4) */
        sum += buf[2] * (seq_t)6; /* a(i-5) */
        sum += buf[1] * (seq_t)2; /* a(i-6) */
        sum -= buf[0] * (seq_t)2; /* a(i-7) */

        /* Shift buffer left by one position. */
        for (idx_t k = 0; k < 6; ++k)
            buf[k] = buf[k + 1];
        buf[6] = sum; /* new a(i) becomes last element */
    }
    return buf[6];
}
