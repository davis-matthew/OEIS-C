/*
 * A126385  Number of base 31 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of base‑31 n‑digit numbers whose adjacent digits differ by at most 1.
   Implements a dynamic‑programming recurrence on the 31‐digit alphabet.
   a(0) = 1 (empty string).  For n ≥ 1, a(n) is computed by iterating walks
   of length n on a path of 31 vertices with a self‑loop at every vertex. */
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                /* empty string */

    /* Arrays of 31 sequence values for current and next length. */
    seq_t f[31];
    seq_t nf[31];

    /* Initialize for length 1: exactly one way to choose each digit. */
    for (int d = 0; d < 31; ++d)
        f[d] = 1;

    /* Iterate lengths 2 … n. */
    for (idx_t len = 2; len <= n; ++len)
    {
        /* Transition: nf[d] = f[d] + f[d-1] + f[d+1] (with bounds). */
        nf[0] = f[0] + f[1];
        for (int d = 1; d <= 29; ++d)
            nf[d] = f[d] + f[d - 1] + f[d + 1];
        nf[30] = f[30] + f[29];

        /* Copy nf into f for the next iteration. */
        for (int d = 0; d < 31; ++d)
            f[d] = nf[d];
    }

    /* Sum over all ending digits. */
    seq_t sum = 0;
    for (int d = 0; d < 31; ++d)
        sum += f[d];

    return sum;
}
