/*
 * A033721  Product theta3(q^d); d | 9.
 * https://oeis.org/A033721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer triples (a,b,c) such that a² + 3 b² + 9
 *            c² = n (a,b,c ∈ ℤ)
 * Range:     u8 n <= 1491, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_idx(idx_t v)
{
    idx_t low = 0, high = v;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == v)
            return mid;
        if (sq < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;  /* floor(sqrt(v)) */
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    idx_t cmax = isqrt_idx(n / 9);
    for (idx_t c = 0; c <= cmax; ++c)
    {
        idx_t weight_c = (c == 0 ? 1 : 2);

        unsigned __int128 csq = (unsigned __int128)c * c;
        unsigned __int128 n1_un = (unsigned __int128)n - (unsigned __int128)9 * csq;
        if (n1_un > (unsigned __int128)UINT64_MAX)
            continue;                 /* n1 fits in idx_t for the intended range */
        idx_t n1 = (idx_t)n1_un;

        idx_t bmax = isqrt_idx(n1 / 3);
        for (idx_t b = 0; b <= bmax; ++b)
        {
            idx_t weight_b = (b == 0 ? 1 : 2);

            unsigned __int128 bsq = (unsigned __int128)b * b;
            unsigned __int128 m2_un = (unsigned __int128)n1 - (unsigned __int128)3 * bsq;
            if (m2_un > (unsigned __int128)UINT64_MAX)
                continue;
            idx_t m2 = (idx_t)m2_un;

            idx_t s = isqrt_idx(m2);
            if ((unsigned __int128)s * s == m2)
            {
                idx_t add_a = (m2 == 0 ? 1 : 2);
                count += (seq_t)(weight_c * weight_b * add_a);
            }
        }
    }

    return count;
}
