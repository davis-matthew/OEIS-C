/*
 * A178139  Distance between n and the next number with the same number of
 *          prime factors (counted with multiplicity).
 * https://oeis.org/A178139
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min d > 0 : A001222(n + d) = A001222(n) * (distance to
 *            the next integer with the same number of prime * factors,
 *            counted with multiplicity)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count the number of prime factors of a uint64_t, counted with multiplicity.
 * This uses simple trial division; efficient enough for n up to about 10 000. */
static seq_t omega_uint64(uint64_t x)
{
    seq_t cnt = 0;
    /* handle factor 2 */
    while ((x & 1ULL) == 0) {
        x >>= 1;
        cnt++;
    }
    /* odd factors */
    for (uint64_t d = 3; d <= x / d; d += 2) {
        while (x % d == 0) {
            x /= d;
            cnt++;
        }
    }
    if (x > 1) {            /* any remaining prime factor > sqrt(original) */
        cnt++;
    }
    return cnt;
}

/* Public function required by the tester. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined for indices below the offset */

    seq_t target = omega_uint64((uint64_t)n);

    for (idx_t m = n + 1; ; ++m) {
        if (omega_uint64((uint64_t)m) == target)
            return (seq_t)(m - n);
    }
}
