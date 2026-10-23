/*
 * A198834  Number of sequences of n coin flips that win on the last flip, if
 *          the sequence of flips ends with (0,1,1) or (1,1,1).
 * https://oeis.org/A198834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n‑1) + a(n‑2) for n > 3;
   a(1) = 0, a(2) = 0, a(3) = 2 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* trivial small indices */
    if (n == 1 || n == 2) return 0;
    if (n == 3) return 2;

    /* a(2) = 0, a(3) = 2 – these are the two values needed to start the recurrence */
    seq_t prev2 = 0;   /* a(i-2)  */
    seq_t prev1 = 2;   /* a(i-1)  */
    seq_t cur;        /* a(i)    */

    for (idx_t i = 4; i <= n; ++i) {
        cur = prev1 + prev2;   /* a(i) = a(i-1) + a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
