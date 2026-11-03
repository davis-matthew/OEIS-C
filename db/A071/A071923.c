/*
 * A071923  a(n) is the prime p such that pi(n^2, (n+1)^2+1) = pi((n+1)^2, p)
 *          where pi(s,t) = pi(t) - pi(s) is the number of primes between s
 *          and t.
 * https://oeis.org/A071923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p > (n+1)^2 such that #primes in
 *            (n^2,(n+1)^2] equals #primes in ((n+1)^2,p]. This is achieved by
 *            counting the primes up to (n+1)^2, then scanning forward for the
 *            C‑th prime after (n+1)^2, where C = pi((n+1)^2) - pi(n^2)
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/prime_pi.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Squares as seq_t to prevent overflow. */
    seq_t n_sq  = (seq_t)n * (seq_t)n;
    seq_t np_sq = (seq_t)(n + 1) * (seq_t)(n + 1);

    /* Count of primes in (n^2,(n+1)^2] */
    seq_t C = prime_pi((idx_t)np_sq) - prime_pi((idx_t)n_sq);

    seq_t count = 0;
    seq_t cand  = np_sq + 1;      /* first integer > (n+1)^2 */

    for (;; ++cand) {
        if (isPrimeGeneric(cand)) {
            ++count;
            if (count == C)
                return cand;
        }
    }
}
