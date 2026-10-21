/*
 * A062818  Values of Moebius transform of perfect numbers, A000396.
 * https://oeis.org/A062818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   compute_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A062818 – Möbius transform of the perfect numbers A000396
    a(n) = sum_{d|n} μ(d) * P(n/d)
    where μ is the Möbius function and P(k) is the kth even perfect
    number (0 if k does not correspond to an existing perfect number).
*/

#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_mu.h"

/* List of exponents p for the first 20 known Mersenne primes 2^p−1
   for which the even perfect numbers 2^(p−1)(2^p−1) are known.
   The sequence is sufficient for all n ≤ 20; for higher indices
   the returned build will give 0 because the corresponding perfect
   numbers are astronomically large and not handled by this routine. */
static const unsigned int mersenne_exp[20] = {
    2,3,5,7,13,17,19,31,61,89,
    107,127,521,607,1279,2203,2281,3217,4253,4423
};

/* Cache of perfect numbers: cache[0] unused, cache[k] holds P(k).
   The cache is grown on demand up to the largest index that
   the caller has required.  When an index beyond the known list
   is requested, 0 is returned. */
static seq_t perfect_cache[1] = {0};   /* cached size is implicit (index 0 missing) */
static size_t perfect_cache_size = 1;  /* number of entries in cache (including unused 0) */

/* Return the kth perfect number (k ≥ 1).  If k is larger than the
   number of computed perfect numbers, 0 is returned.  The function
   expands the cache as necessary.  For the known list the largest
   computed perfect number is P(20); for higher indices no perfect
   number is available. */
static seq_t perfect_number(idx_t k) {
    if (k == 0) return 0;
    if (k + 1 > perfect_cache_size) {
        size_t new_size = (size_t)k + 1;
        if (new_size > 20 + 1) {
            /* index beyond known perfect numbers – return 0 */
            return 0;
        }
        /* enlarge cache and compute new perfect numbers up to k */
        while (perfect_cache_size <= k) {
            unsigned int p = mersenne_exp[perfect_cache_size - 1]; /* exponent for next perfect number */
            seq_t pow2 = 1;
            for (unsigned int i = 0; i < p; ++i) pow2 *= 2;          /* 2^p */
            seq_t m_minus_one = pow2 - 1;                            /* 2^p – 1 */
            seq_t pow2_half = pow2 / 2;                              /* 2^(p‑1) */
            seq_t perfect = m_minus_one * pow2_half;                 /* even perfect number */
            perfect_cache[perfect_cache_size] = perfect;
            ++perfect_cache_size;
        }
    }
    return perfect_cache[k];
}

/* Main sequence function.  Computes a(n) = Σ_{d|n} μ(d) * P(n/d). */
seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Allocate static buffers for μ and primes up to 10000. */
    static int8_t mu[10001];
    static int primes[10001];
    static int prime_cnt = 0;

    compute_mu(n, mu, primes, &prime_cnt);

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* First divisor d */
        if (mu[d] != 0) {
            idx_t k = n / d;
            seq_t p = perfect_number(k);
            if (mu[d] > 0)
                sum += p;
            else
                sum -= p;
        }

        /* Second divisor n/d if distinct */
        idx_t d2 = n / d;
        if (d2 != d && d2 != 0) {
            if (mu[d2] != 0) {
                idx_t k = n / d2;
                seq_t p = perfect_number(k);
                if (mu[d2] > 0)
                    sum += p;
                else
                    sum -= p;
            }
        }
    }
    return sum;
}
