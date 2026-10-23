/*
 * A042974  n 1's followed by a 2.
 * https://oeis.org/A042974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = 1 if (n - S_{k-1}) <= k else 2 where k is the smallest
 *            integer with S_k >= n and S_k = k*(k+3)/2 (cumulative length
 *            after block k)
 * Range:     u8 n <= 8157, u16 n <= 8157, u32 n <= 8157, u64 n <= 8157,
 *            u128 n <= 8157, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find block k that contains position n */
    idx_t k   = 0;
    idx_t cum = 0;          /* cum = S_k */
    while (cum < n) {
        ++k;
        cum += k + 1;       /* block k has length k+1 */
    }

    /* Position of the first element of block k */
    idx_t first_in_block = cum - (k + 1);
    idx_t offset = n - first_in_block;  /* 1‑based offset within block */

    return (seq_t)(offset <= k ? 1 : 2);
}
