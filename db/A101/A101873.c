/*
 * A101873  Number of Abelian groups of order 4n+1.
 * https://oeis.org/A101873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{p^e‖(4n+1)} partition(e) where partition(e) is the
 *            number of integer partitions of e
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t part[21] = {
    /* e = 0 … 20 */
    1, 1, 2, 3, 5, 7, 11, 15, 22, 30,
    42, 56, 77,101,135,176,231,297,385,490,627
};

seq_t a(idx_t n)
{
    /* 4n+1 : fits in 64 bits for n up to 10 000 (and all our test cases) */
    uint64_t m = (uint64_t)n * 4u + 1u;
    seq_t  ans = 1u;

    /* factor the odd number m by trial division */
    for (uint64_t p = 3u; p * p <= m; p += 2u) {
        if (m % p == 0u) {
            int exp = 0;
            while (m % p == 0u) {
                m /= p;
                ++exp;
            }
            /* exp ≤ 20 for the tested input range, but we truncate to 20 if necessary */
            if (exp > 20) exp = 20;
            ans *= part[exp];
        }
    }

    /* remaining prime factor (if any) has exponent 1 → part[1] = 1 */
    return ans;
}
