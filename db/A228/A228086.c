/*
 * A228086  a(n) is the least k which satisfies n = k + bitcount(k), or 0 if
 *          no such k exists. Here bitcount(k) (or wt(k), A000120) gives the
 *          number of 1's in binary representation of nonnegative integer k.
 * https://oeis.org/A228086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least k such that n = k + bitcount(k), or 0 if no
 *            such k exists. This implements a straightforward O(n) search for
 *            each n, suitable for n up to ~10⁴
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Count the number of set bits in an idx_t value. */
static inline size_t popcount_u64(idx_t x)
{
    size_t c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* n = 0 is a special case: k = 0 satisfies 0 = 0 + bitcount(0). */
    if (n == 0)
        return (seq_t)0;

    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t bc = popcount_u64(k);
        if (k + bc == n)
            return (seq_t)k;
    }
    return (seq_t)0; /* no such k exists */
}
