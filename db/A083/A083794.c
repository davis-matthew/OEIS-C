/*
 * A083794  Numbers k such that tau(k) is different from tau(k-1), where
 *          tau(m) = number of divisors of m.
 * https://oeis.org/A083794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching for k such that τ(k) ≠ τ(k‑1) */
#include <stdint.h>

static inline uint64_t divisor_count_u64(uint64_t n)
{
    if (n <= 1) return 1;          /* τ(1) = 1 */
    uint64_t count = 1;
    uint64_t exp;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            exp = 0;
            while (n % p == 0)
            {
                n /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
    }
    if (n > 1) count *= 2;          /* remaining prime factor */
    return count;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard for safety */
    idx_t found = 0;
    uint64_t prev_tau = 0;         /* τ(0) treated as 0 */

    for (uint64_t k = 1;; ++k)
    {
        uint64_t tau = divisor_count_u64(k);

        if (k == 1)
        {
            /* τ(1) ≠ τ(0) by definition, so the first term is 1 */
            if (++found == n) return (seq_t)k;
            prev_tau = tau;
            continue;
        }

        if (tau != prev_tau)
        {
            if (++found == n) return (seq_t)k;
        }
        prev_tau = tau;
    }
}
