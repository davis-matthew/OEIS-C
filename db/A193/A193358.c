/*
 * A193358  a(0)=1; a(1)=2 and for n>1: a(n)=a(n-a(n-2))+2.
 * https://oeis.org/A193358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(n)=a(n-a(n-2))+2 for n>1. The sequence can be
 *            represented by blocks: for every k≥0, indices from k*(k+1) to
 *            k*(k+1)+2*(k+1)-1 contain 2*k terms of value 2*(k+1), followed
 *            by 2*k+1 and 2*(k+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Find block k such that k*(k+1) ≤ n < (k+1)*(k+2).  */
    double sqrt_val = sqrt((double)(4.0 * (long double)n + 1.0));
    idx_t k = (idx_t)((-1.0 + sqrt_val) / 2.0);

    /* Adjust k in case of floating‑point error.  Use 128‑bit
       arithmetic to avoid overflow. */
    while ((unsigned __int128)k * (k + 1) > n)
        k--;
    while ((unsigned __int128)(k + 1) * (k + 2) <= n)
        k++;

    /* Position within the block. */
    unsigned __int128 start = (unsigned __int128)k * (k + 1);
    idx_t offset = n - (idx_t)start;  /* offset < 2*(k+1) */

    /* Compute the value according to the block pattern. */
    if (offset < 2 * k)
    {
        return (seq_t)(2 * (k + 1));
    }
    else if (offset == 2 * k)
    {
        return (seq_t)(2 * k + 1);
    }
    else /* offset == 2*k + 1 */
    {
        return (seq_t)(2 * (k + 1));
    }
}
