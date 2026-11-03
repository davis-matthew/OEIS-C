/*
 * A218219  Define a(x,y) to be 1 if x is 0 or 1 and y*a(x-1,y)-a(x-2,y)
 *          otherwise. Then the n-th term of the sequence is a(n,n).
 * https://oeis.org/A218219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n,n) where a(x,y) = 1 if x ≤ 1 = y * a(x‑1,y) –
 *            a(x‑2,y) otherwise (OEIS A218219)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for size_t */

static seq_t compute_column(idx_t y)
{
    /* a(0,y) = 1, a(1,y) = 1 */
    seq_t prev2 = 1;   /* a(0,y) */
    seq_t prev1 = 1;   /* a(1,y) */
    if (y <= 1)
        return prev1;  /* y = 0 or 1 */

    /* iterate from x = 2 up to x = y */
    for (idx_t x = 2; x <= y; ++x)
    {
        seq_t cur = ((seq_t) y) * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;      /* a(y,y) */
}

seq_t a(idx_t n)
{
    return compute_column(n);
}
