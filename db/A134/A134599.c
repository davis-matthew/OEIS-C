/*
 * A134599  Sum of digital sums (base 3) of the prime factors of n.
 * https://oeis.org/A134599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of the base‑3 digit sums of the prime factors of n
 *            (counted with multiplicity). For each occurrence of a prime
 *            factor p in n, add the sum of the base‑3 digits of p
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_base3_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum_base3_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t m = (uint64_t)n;   /* work locally in 64‑bit */
    seq_t   res = 0;

    /* factor 2 multiplicatively */
    while ((m & 1ULL) == 0)
    {
        res += 2;       /* digit_sum_base3_u64(2) == 2 */
        m >>= 1;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= m; p += 2)
        while (m % p == 0)
        {
            res += digit_sum_base3_u64(p);
            m /= p;
        }

    /* remaining prime factor > 1 */
    if (m > 1)
        res += digit_sum_base3_u64(m);

    return res;
}
