/*
 * A227291  Characteristic function of squarefree numbers squared (A062503).
 * https://oeis.org/A227291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 iff n is a perfect square and its square root is
 *            squarefree, else 0
 * Range:     u8 n <= 9161, u16 n <= 9161, u32 n <= 9161, u64 n <= 9161,
 *            u128 n <= 9161, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Check whether m is squarefree, using trial division. */
static bool is_squarefree_uint64(uint64_t m)
{
    if (m <= 1) return true;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int count = 0;
            while (m % p == 0) {
                m /= p;
                ++count;
                if (count > 1) return false;
            }
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    uint64_t nn = (uint64_t)n;

    /* Integer square‑root by binary search (avoids floating point). */
    uint64_t low = 0, high = nn, r = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == nn) { r = mid; break; }
        if (sq < nn) { low = mid + 1; r = mid; }
        else        { high = mid - 1; }
    }

    /* Check perfect square. */
    if ((__uint128_t)r * r != nn) return (seq_t)0;

    /* Return 1 if sqrt(n) is squarefree, else 0. */
    return is_squarefree_uint64(r) ? (seq_t)1 : (seq_t)0;
}
