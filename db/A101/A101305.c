/*
 * A101305  Begin with 0 and at each successive iteration append the next
 *          power of 10.
 * https://oeis.org/A101305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(n) = a(n-1)*10^(n+1) + 10^n (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t res   = 0;          /* running result */
    seq_t block = pow10(1);   /* 10^k, starts with k = 1 */
    seq_t shift = pow10(2);   /* 10^(k+1), starts with k = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        res   = res * shift + block; /* append current block */
        block *= 10;                 /* next 10^k   -> 10^(k+1) */
        shift *= 10;                 /* next 10^(k+1) -> 10^(k+2) */
    }
    return res;
}
