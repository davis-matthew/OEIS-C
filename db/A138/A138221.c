/*
 * A138221  a(n) = the smallest divisor of n that is >= the number of positive
 *          divisors of n.
 * https://oeis.org/A138221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the smallest divisor d of n such that d ≥ τ(n),
 * where τ(n) is the number of positive divisors of n.
 * Sequence A138221.
 */

#include <stdint.h>

/* Compute the number of divisors of a 64‑bit integer n. */
static uint64_t divisor_count_uint64(uint64_t n)
{
    uint64_t tau = 1;
    uint64_t m   = n;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }
    if (m > 1)          /* remaining prime factor */
        tau *= 2;
    return tau;
}

seq_t a(idx_t n)
{
    /* Fast path for n = 1 */
    if (n == 1) return (seq_t)1;

    uint64_t nn  = (uint64_t)n;
    uint64_t tau = divisor_count_uint64(nn);

    /* Search for the smallest divisor ≥ τ */
    seq_t best = (seq_t)0;
    uint64_t limit = 0;
    for (uint64_t i = 1; i * i <= nn; ++i) {
        if (nn % i == 0) {
            uint64_t d1 = i;
            uint64_t d2 = nn / i;

            if (d1 >= tau && (best == 0 || d1 < (uint64_t)best))
                best = (seq_t)d1;
            if (d2 >= tau && (best == 0 || d2 < (uint64_t)best))
                best = (seq_t)d2;
        }
    }

    /* best must be found; n itself is a divisor, so this suffices */
    return best;
}
