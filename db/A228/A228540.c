/*
 * A228540  Rows of negated binary Walsh matrices interpreted as reverse
 *          binary numbers.
 * https://oeis.org/A228540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = row k of the negated binary Walsh matrix of order 2^m,
   where m = floor(log2(n+1)), interpreted as a reverse binary number. */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Determine m = floor(log2(n+1))  */
    idx_t np1 = n + 1;
    idx_t m;
    if (np1 == 0) {           /* overflowed only for n = UINT64_MAX */
        m = 63;                /* max for 64‑bit unsigned */
    } else {
        m = 63 - __builtin_clzll((unsigned long long)np1);
    }

    /* Compute row index k and matrix size 2^m */
    idx_t start = ((idx_t)1 << m) - 1;
    idx_t k = n - start;
    idx_t size = ((idx_t)1 << m);

    /* Build the reverse binary value */
    seq_t result = 0;
    for (idx_t j = 0; j < size; ++j) {
        if ((__builtin_parityll((unsigned long long)(k & j))) == 0) {
            result |= pow2(j);
        }
    }
    return result;
}
