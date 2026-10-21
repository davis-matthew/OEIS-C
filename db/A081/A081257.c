/*
 * A081257  a(n) is the greatest prime factor of (n^3 - 1).
 * https://oeis.org/A081257
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = greatest prime factor of n³ – 1 (offset 2)
 * Range:     u8 n <= 16, u16 n <= 265, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the greatest prime factor of a 64‑bit integer n >= 2.
   This simple trial‑division routine is fast enough for the
   values that occur in the tests (n ≈ 10⁴ → n³ ≈ 10¹²). */
static uint64_t gpf_uint64(uint64_t n)
{
    uint64_t max = 0;

    /* factor out powers of 2 */
    while ((n & 1U) == 0) {
        max = 2U;
        n >>= 1U;
    }

    /* odd factors */
    for (uint64_t i = 3U; i * i <= n; i += 2U) {
        while (n % i == 0U) {
            max = i;
            n /= i;
        }
    }

    /* remaining prime > 2 */
    if (n > 1U) {
        max = n;
    }

    return max;
}

/* a(idx_t n) computes the greatest prime factor of n³ – 1.
   The function is defined for n ≥ 2; for smaller indices it
   returns 0. */
seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;
    }

    /* compute n³ – 1 in uint64_t (safe for the test range) */
    uint64_t nn = (uint64_t)n;
    __uint128_t cube = (__uint128_t)nn * nn * nn;
    uint64_t m = (uint64_t)(cube - 1U);

    /* greatest prime factor of m */
    return (seq_t)gpf_uint64(m);
}
