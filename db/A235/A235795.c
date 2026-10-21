/*
 * A235795  Triangle read by rows T(n,k) in which row n gives the trajectory
 *          of n in Collatz problem including the trajectory [1, 4, 2, 1] for
 *          n = 1.
 * https://oeis.org/A235795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 317, u16 n <= 9358, u32 n <= 9358, u64 n <= 9358,
 *            u128 n <= 9358, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th element of the flattened triangle of Collatz trajectories:
   row m gives the Collatz sequence of m ending at 1 (including that 1),
   with the special case row 1 = 1,4,2,1. */
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t pos = 1; /* next output position (1‑indexed) */

    for (idx_t start = 1; ; ++start)
    {
        if (start == 1)
        {
            /* special row for start = 1: 1,4,2,1 */
            seq_t tri[4] = { (seq_t)1, (seq_t)4, (seq_t)2, (seq_t)1 };
            for (size_t t = 0; t < 4; ++t)
            {
                if (pos == n) return tri[t];
                ++pos;
            }
            continue;
        }

        seq_t val = (seq_t)start;
        while (1)
        {
            if (pos == n) return val;
            ++pos;

            if (val == (seq_t)1) break;

            if ((val & (seq_t)1) == (seq_t)0)
                val = val >> 1;
            else
                val = val * (seq_t)3 + (seq_t)1;
        }
    }

    /* unreachable, just to keep the compiler happy */
    return (seq_t)0;
}
