/*
 * A181830  The number of positive integers <= n that are strongly prime to n.
 * https://oeis.org/A181830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(n) - τ(n‑1) for n ≥ 2, a(0) = a(1) = 0 * where φ is
 *            Euler’s totient and τ is the divisor‑count function. * The
 *            implementation uses elementary trial division and works for *
 *            any n that fits in the idx_t type (uint64_t). For the seq_t *
 *            return type it relies only on the built‑in arithmetic...
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ----- helper: integer square root (floor) for idx_t ----- */
static idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while (r <= n / r) {
        ++r;
    }
    return r - 1;
}

/* ----- Euler's totient φ(n) ----- */
static seq_t phi_u64(idx_t n)
{
    if (n == 0) return 0;
    seq_t result = (seq_t)n;
    idx_t m = n;
    /* factor 2 */
    if ((m & 1) == 0) {
        result -= result / 2;
        while ((m & 1) == 0) m >>= 1;
    }
    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            result -= result / (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {
        result -= result / (seq_t)m;
    }
    return result;
}

/* ----- divisor‑count τ(n) ----- */
static seq_t divisor_count_u64(idx_t n)
{
    if (n == 0) return 0;
    seq_t count = 1;
    idx_t m = n;
    /* factor 2 */
    if ((m & 1) == 0) {
        unsigned exp = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++exp;
        }
        count *= (seq_t)(exp + 1);
    }
    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            count *= (seq_t)(exp + 1);
        }
    }
    if (m > 1) {
        count *= 2;                 /* remaining prime factor exponent 1 */
    }
    return count;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t phi_val = phi_u64(n);
    seq_t tau_val = divisor_count_u64(n - 1);
    return phi_val - tau_val;
}
