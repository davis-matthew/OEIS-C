/*
 * A226430  The number of simple permutations of length n which avoid 1243 and
 *          2431.
 * https://oeis.org/A226430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A226430: number of simple permutations of length n avoiding 1243 and 2431.
   The sequence satisfies the 5‑term linear recurrence obtained from its
   generating function:  (1-2x)(1-x-x²)²·G(x)=x-2x²-5x³+12x⁴+x⁵-8x⁶-3x⁷.
   For n ≥ 8:
          a(n) = 4·a(n-1) - 3·a(n-2) - 4·a(n-3) + 3·a(n-4) + 2·a(n-5)
   Initial values (offset 1):
          a(1)=1, a(2)=2, a(3)=0, a(4)=2, a(5)=4, a(6)=10, a(7)=21. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[8] = {0, 1, 2, 0, 2, 4, 10, 21};
    if (n <= 7)
        return init[n];

    /* Initialise last five values: a(7), a(6), ..., a(3) */
    seq_t last[5] = {init[7], init[6], init[5], init[4], init[3]};

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = 4 * last[0]
                   - 3 * last[1]
                   - 4 * last[2]
                   + 3 * last[3]
                   + 2 * last[4];
        /* rotate the ring buffer */
        last[4] = last[3];
        last[3] = last[2];
        last[2] = last[1];
        last[1] = last[0];
        last[0] = next;
    }
    return last[0];
}
