/*
 * A186495  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=5i (A008587) and g(j)=j-th
 *          pentagonal number (A000326). Complement of A186496.
 * https://oeis.org/A186495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊(⌊√(1 + 120 n)⌋ + 1)/6⌋
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square‑root for unsigned __int128, binary search */
static inline idx_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64) - 1;
    while (low <= high)
    {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == x)
            return (idx_t)mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (idx_t)high;   /* floor sqrt */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;         /* undefined for offset 1, but keeps type safety */

    unsigned __int128 D = (unsigned __int128)120 * n + 1;   /* 1 + 120n */
    idx_t sqrtD = isqrt_u128(D);                          /* ⌊√(1+120n)⌋ */

    idx_t cnt = (sqrtD + 1) / 6;                         /* number of pentagonal numbers < 5n */

    return (seq_t)n + (seq_t)cnt;
}
