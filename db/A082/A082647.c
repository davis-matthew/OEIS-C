/*
 * A082647  Number of ways n can be expressed as the sum of d consecutive
 *          positive integers where d>0 is a divisor of n.
 * https://oeis.org/A082647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of odd divisors d of n with d ≤ ⌊√(2n)⌋
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/count_divs.h"

/* --------- utility: integer square root for uint128 values --------- */
static uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;   /* √(2⁶⁴) < 2³² */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* --------- public sequence function --------- */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset starts at 1 */
        return 0;

    /* limit = floor(√(2n)) */
    uint64_t limit = isqrt_uint128((unsigned __int128)n * 2ULL);

    /* factor n into odd prime powers */
    uint64_t pr[32], exp[32];
    size_t cnt = 0;
    uint64_t temp = n;

    /* remove all factors of 2 – they do not affect odd divisors */
    while ((temp & 1) == 0)      /* temp % 2 == 0 */
        temp >>= 1;

    /* trial division for odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            do {
                temp /= p;
                ++e;
            } while (temp % p == 0);
            pr[cnt] = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {               /* remaining prime factor */
        pr[cnt] = temp;
        exp[cnt] = 1U;
        ++cnt;
    }

    /* enumerate odd divisors up to the limit */
    seq_t result = 0;
    count_divs(0, 1, pr, exp, cnt, limit, &result);
    return result;
}
