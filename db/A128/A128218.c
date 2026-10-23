/*
 * A128218  First differences of A128217.
 * https://oeis.org/A128218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for all n except at indices S_k = k(k‑1)/2 + 1. * At n
 *            = S_k, a(n) = k if k is odd, k+1 if k is even
 * Range:     u8 n <= 9285, u16 n <= 9285, u32 n <= 9285, u64 n <= 9285,
 *            u128 n <= 9285, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)             /* offset 1; out of range */
        return (seq_t)0;

    /* Find the smallest k such that S_k > n, where S_k = k(k-1)/2 + 1 */
    idx_t low = 1;
    idx_t high = 2*n + 10; /* safely above the required bound */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t s_mid = mid * (mid - 1) / 2 + 1;
        if (s_mid > n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t k = low;              /* first k with S_k > n */
    idx_t block = k - 1;        /* block that contains n */
    idx_t s_block = block * (block - 1) / 2 + 1;

    if (n == s_block) { /* n is the starting position of a block */
        if ((block & 1) == 0)  /* even block index */
            return (seq_t)(block + 1);
        else                   /* odd block index */
            return (seq_t)block;
    } else
        return (seq_t)1;    /* all other positions are 1 */
}
