/*
 * A089655  a(1)=1 and for n>=2 a(n) is the denominator of A(n) (see comment
 *          for A(n) definition).
 * https://oeis.org/A089655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 – 1) / rad(n^2 – 1) where rad(m) is the product of
 *            the distinct prime factors of m (i.e. a(n) = ∏ p^(e–1) over all
 *            primes p^e || n^2–1). The function uses factor_u64 from the
 *            utilities to factor n^2–1 and multiplies the appropriate prime
 *            powers
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* for size_t */
#include <stdint.h>     /* for uint64_t */
#include "../utils/factor_u64.h"   /* prototype of factor_u64 */

seq_t a(idx_t n)
{
    if (n <= 1)     /* a(1) = 1 by definition */
        return (seq_t)1;

    /* m = n^2 – 1 */
    uint64_t m = (uint64_t)n * (uint64_t)n - 1ULL;

    /* Factor m.  The maximum number of prime factors for m ≤ 1e8
       is well below 10, so a small fixed array suffices. */
    uint64_t primes[64], exponents[64];
    size_t factor_cnt = 0;
    factor_u64(m, primes, exponents, &factor_cnt);

    /* Compute the product of p^(exp–1) for every prime factor. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exponents[i];
        /* multiply p exp–1 times */
        for (uint64_t k = 1; k < exp; ++k)
            result *= (seq_t)p;
    }
    return result;
}
