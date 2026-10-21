/*
 * A055054  Numbers of the form 4^i*(8j+7) or 4^i*(3j+1).
 * https://oeis.org/A055054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   in_sequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A055054: Numbers not of the form 2x²+3y²+6z².
   A(n) is the n‑th positive integer that can be written as 4ᶦ·(8j+7) or 4ᶦ·(3j+1)
   (i,j ≥ 0).  Offset is 1. */
#include <stdbool.h>
#include "../utils/in_sequence.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, return 0 */
    idx_t count = 0;
    seq_t cand  = 1;                /* first positive integer */

    while (count < n)
    {
        if (in_sequence(cand))
            ++count;
        if (count == n) return cand;
        ++cand;
    }
    return 0; /* unreachable */
}
