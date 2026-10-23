/*
 * A195986  Exponent of the largest power of 2 that divides 5^n - 3^n.
 * https://oeis.org/A195986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is odd, otherwise a(n) = v2(n) + 3 * where v2(n)
 *            is the exponent of the largest power of 2 dividing n
 * Range:     u8 n <= 9468, u16 n <= 9468, u32 n <= 9468, u64 n <= 9468,
 *            u128 n <= 9468, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the exponent of the largest power of 2 dividing n. */
static inline seq_t v2_uint64(idx_t n)
{
    seq_t cnt = 0;
    while ((n & 1) == 0)
    {
        cnt++;
        n >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1 */
    if (n & 1) /* n is odd */
        return (seq_t)1;
    /* n is even */
    return v2_uint64(n) + (seq_t)3;
}
