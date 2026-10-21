/*
 * A083718  a(n) = (greatest prime <= n) + (greatest prime factor of n), with
 *          a(1) = 2.
 * https://oeis.org/A083718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007917(n) + A006530(n) (greatest prime ≤ n) + (greatest
 *            prime factor of n) with a(1) = 2
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int is_prime_uint64(uint64_t x)
{
    if (x < 2) return 0;
    if (x == 2 || x == 3) return 1;
    if ((x & 1ULL) == 0) return 0;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

/* greatest prime ≤ n, n ≥ 2 */
static inline uint64_t prime_floor_uint64(uint64_t n)
{
    for (uint64_t x = n; x >= 2; --x)
        if (is_prime_uint64(x)) return x;
    return 2; /* fallback for safety */
}

/* greatest prime factor of n, n ≥ 2 */
static inline uint64_t largest_prime_factor_uint64(uint64_t n)
{
    uint64_t largest = 1;
    while ((n & 1ULL) == 0) { largest = 2; n >>= 1; }
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            largest = d;
            while (n % d == 0) n /= d;
        }
    }
    if (n > 1) largest = n;
    return largest;
}

seq_t a(idx_t n)
{
    if (n == 1) return 2; /* a(1) is defined as 2 */

    /* n fits in uint64_t because idx_t is uint64_t */
    uint64_t nn = (uint64_t)n;

    uint64_t gp  = prime_floor_uint64(nn);          /* greatest prime ≤ n   */
    uint64_t gpf = largest_prime_factor_uint64(nn); /* greatest prime factor */

    return (seq_t)(gp + gpf);
}
