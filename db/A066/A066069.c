/*
 * A066069  a(n) is the smallest positive integer m such that n divides (n +
 *          m)^m.
 * https://oeis.org/A066069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = smallest m > 0 such that n | m^m
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The condition n | m^m is equivalent to n | (n+m)^m because (n+m) ≡ m (mod n). */
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

/* Compute a(n) for n >= 1.  n is an idx_t but cast to uint64_t for factorization. */
seq_t a(idx_t n)
{
    /* Special case: n = 1 always has a(1) = 1 */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Factor n.  The maximum number of distinct primes for u64 is < 16,
       but we use a generous buffer size to be safe. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Search for the smallest m satisfying (v_p(m) * m) >= exponent for all primes. */
    for (uint64_t m = 1; ; ++m) {
        bool ok = true;
        for (size_t i = 0; i < pcnt && ok; ++i) {
            uint64_t p = primes[i];
            uint64_t e = exps[i];
            /* Compute v_p(m). */
            uint64_t temp = m;
            uint64_t vp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++vp;
            }
            /* Need vp * m >= e. */
            if (vp * m < e)
                ok = false;
        }
        if (ok)
            return (seq_t)m;
    }
}
