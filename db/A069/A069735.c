/*
 * A069735  Number of regular orientable coverings of the Klein bottle with 2n
 *          lists.
 * https://oeis.org/A069735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = d(n) (n odd) * a(n) = d(n) + d(n/2) (n even) * where d(n)
 *            is the number of positive divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n < 2^64 */
static inline uint64_t integer_sqrt(uint64_t n)
{
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Count positive divisors of m using trial division. */
static inline uint64_t divisor_count_u64(uint64_t m)
{
    if (m == 0) return 0;            /* undefined, but never called for n<1   */
    uint64_t cnt = 1;
    uint64_t tmp = m;

    /* factor out 2 */
    uint64_t exp = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        exp++;
    }
    if (exp > 0) cnt *= (exp + 1);

    /* factor odd primes */
    uint64_t limit = integer_sqrt(tmp);
    for (uint64_t p = 3; p <= limit; p += 2) {
        if (tmp % p == 0) {
            exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            cnt *= (exp + 1);
            limit = integer_sqrt(tmp);
        }
    }
    if (tmp > 1)                 /* remaining prime */
        cnt *= 2;

    return cnt;
}

seq_t a(idx_t n)
{
    uint64_t d_n = divisor_count_u64((uint64_t)n);
    if ((n & 1ULL) == 0) {               /* even n */
        uint64_t d_half = divisor_count_u64((uint64_t)(n / 2));
        return (seq_t)(d_n + d_half);
    } else {                              /* odd n */
        return (seq_t)d_n;
    }
}
