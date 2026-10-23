/*
 * A027562  Number of 5-unbalanced strings of length n (=2^n-A027560(n)).
 * https://oeis.org/A027562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3·a(n‑1) + 3·a(n‑2) – 14·a(n‑3) * + 2·a(n‑4) + 15·a(n‑5)
 *            – 6·a(n‑6) (n ≥ 7) * Initial values:
 *            a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=0, a(6)=2 * Compile-time typeseq_t
 *            and idx_t are supplied by the build. * This implementation is
 *            O(n) per call and uses only a small fixed * amount of memory,
 *            suitabl...
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n <= 5) return (seq_t)0;
    if (n == 6) return (seq_t)2;

    /* prev[0..6] will hold a_{i-6}, a_{i-5}, … , a_i at each iteration. */
    seq_t prev[7] = { 0, 0, 0, 0, 0, 0, (seq_t)2 };   /* a0..a5 = 0, a6 = 2 */

    for (idx_t i = 7; i <= n; ++i)
    {
        /* Compute a_i from the recurrence. */
        seq_t next = 3 * prev[6] +                 /* a_{i-1} */
                     3 * prev[5] +                 /* a_{i-2} */
                    -14 * prev[4] +                 /* a_{i-3} */
                     2 * prev[3] +                 /* a_{i-4} */
                    15 * prev[2] +                 /* a_{i-5} */
                    - 6 * prev[1];                /* a_{i-6} */

        /* Shift the window: drop a_{i-6} and make room for a_i. */
        for (int k = 0; k < 6; ++k)
            prev[k] = prev[k + 1];
        prev[6] = next;
    }

    return prev[6];
}
