/*
 * A145390  Number of sublattices of index n of a centered rectangular lattice
 *          fixed by a reflection.
 * https://oeis.org/A145390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = d(n) if n odd; * a(n) = d(n) - d(n/2) if n ≡ 2 (mod 4); *
 *            a(n) = d(n) - d(n/2) + 2*d(n/4) if n divisible by 4 * where d(x)
 *            is the number of divisors of x
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count number of divisors of a 64‐bit unsigned integer. */
static uint64_t divisor_count_uint64(uint64_t n)
{
    uint64_t d = 0;
    for (uint64_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (i * i == n) d += 1;      /* perfect square */
            else            d += 2;      /* pair (i, n/i) */
        }
    }
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for 0, but return 0 */

    uint64_t dn   = divisor_count_uint64(n);
    uint64_t dn2  = 0;
    uint64_t dn4  = 0;
    seq_t     res = (seq_t)dn;

    if ((n & 1ULL) == 0) {               /* n even */
        dn2 = divisor_count_uint64(n >> 1);
        res = (seq_t)dn - (seq_t)dn2;
        if ((n & 3ULL) == 0) {           /* n divisible by 4 */
            dn4 = divisor_count_uint64(n >> 2);
            res = res + (seq_t)(2ULL * dn4);
        }
    }

    return res;
}
