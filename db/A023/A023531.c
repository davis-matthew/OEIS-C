/*
 * A023531  a(n) = 1 if n is of the form m(m+3)/2, otherwise 0.
 * https://oeis.org/A023531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, nice
 * Formula:   a(n) = 1 if n = m(m+3)/2 for some integer m ≥ 0, otherwise 0
 * Range:     u8 n <= 9788, u16 n <= 9788, u32 n <= 9788, u64 n <= 9788,
 *            u128 n <= 9788, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit value, result fits in uint64_t.
   A simple binary search is sufficient and uses only integer arithmetic. */
static inline uint64_t isqrt128(__int128 x)
{
    uint64_t low = 0, high = ((uint64_t)1 << 35);  /* sqrt(8·UINT64_MAX) < 2^35 */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __int128 prod = (__int128)mid * mid;
        if (prod == x)
            return mid;
        else if (prod < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor sqrt */
}

seq_t a(idx_t n)
{
    /* Compute the discriminant d = 9 + 8·n. */
    __int128 d = (__int128)9 + (__int128)8 * (__int128)n;

    /* Determine if d is a perfect square. */
    uint64_t s = isqrt128(d);
    if ((__int128)s * (__int128)s != d)
        return (seq_t)0;  /* discriminant not a perfect square */

    /* Check that (s – 3) is non‑negative and even, yielding integer m. */
    int64_t t = (int64_t)s - 3;
    if (t < 0 || (t & 1))
        return (seq_t)0;

    return (seq_t)1;
}
