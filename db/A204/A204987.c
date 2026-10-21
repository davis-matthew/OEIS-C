/*
 * A204987  Least k such that n divides 2^k - 2^j for some j satisfying 1 <= j
 *          < k.
 * https://oeis.org/A204987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (t>0 ? t : 1) + ord_2(m), where n = 2^t * m with m odd,
 *            and ord_2(m) is the smallest r≥1 such that m | 2^r – 1. +
 *            Handles n ≥ 1
 * Range:     u8 n <= 268, u16 n <= 6556, u32 n <= 6556, u64 n <= 6556,
 *            u128 n <= 6556, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Decompose n into 2^t * m with m odd */
    uint64_t t = 0;
    uint64_t temp = (uint64_t)n;
    while ((temp & 1ULL) == 0ULL) {
        t++;
        temp >>= 1;
    }
    uint64_t m = temp;   /* now m is odd (unless n=0, not possible) */

    /* Compute the multiplicative order of 2 modulo m */
    uint64_t r;
    if (m == 1ULL) {
        r = 1ULL;                   /* 2^1 ≡ 1 (mod 1) */
    } else {
        uint64_t val = 2ULL % m;    /* 2^1 mod m */
        r = 1ULL;
        while (val != 1ULL) {
            val = (val * 2ULL) % m;
            ++r;
        }
    }

    uint64_t add = (t > 0ULL) ? t : 1ULL;
    return (seq_t)(r + add);
}
