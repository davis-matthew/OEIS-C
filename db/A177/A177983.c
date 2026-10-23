/*
 * A177983  a(1)=3. Otherwise the average of the least prime divisors of 2n-1
 *          and 2n+3.
 * https://oeis.org/A177983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (lp(2n-1)+lp(2n+3))/2 for n >= 2, a(1)=3 where lp(m) =
 *            smallest prime divisor of m (m > 1)
 * Range:     u8 n <= 63, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Smallest prime divisor of an odd uint64_t (assumes n ≥ 3) */
static uint64_t _smallest_prime_factor_uint64(uint64_t n)
{
    if (n % 2ull == 0ull) return 2ull;

    for (uint64_t d = 3ull; d * d <= n; d += 2ull)
        if (n % d == 0ull)
            return d;

    return n;          /* n is prime */
}

/* Sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1)                  /* defined value */
        return (seq_t)3;

    /* Compute 2n‑1 and 2n+3 – they fit in uint64_t for the required range */
    uint64_t two_n_minus_1 = (uint64_t)2 * (uint64_t)n - 1ull;
    uint64_t two_n_plus_3  = (uint64_t)2 * (uint64_t)n + 3ull;

    uint64_t lp1 = _smallest_prime_factor_uint64(two_n_minus_1);
    uint64_t lp2 = _smallest_prime_factor_uint64(two_n_plus_3);

    /* average, stored in seq_t */
    seq_t sum = (seq_t)lp1 + (seq_t)lp2;
    return sum / (seq_t)2;
}
