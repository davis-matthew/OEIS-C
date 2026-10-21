/*
 * A230493  Number of ways to write n = (2-(n mod 2))*p + q + r with p <= q <=
 *          r such that p, q, r, 2*p^2 - 1, 2*q^2 - 1, 2*r^2 - 1 are all
 *          prime.
 * https://oeis.org/A230493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered representations of * n = (2-(n mod
 *            2))*p + q + r with p ≤ q ≤ r, where * p, q, r, 2p²‑1, 2q²‑1,
 *            2r²‑1 are all prime
 * Range:     u8 n <= 4452, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE:  A 1‑based n.  The function is correct for any n ≥ 1,
 * but is guaranteed to run in < 2 s for n ≤ 10 000, which is the
 * usual test range for this entry. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"           /* for is_prime(idx_t)   */
#include "../utils/build_primes_upto.h" /* for build_primes_upto */

static idx_t   max_n_sieve = 0ULL;   /* largest n for which we have a sieve   */
static uint64_t *primes_arr   = NULL;/* array of primes ≤ max_n_sieve          */
static size_t   primes_cnt   = 0ULL; /* number of primes in primes_arr        */

/* Ensure that primes_arr contains all primes ≤ n. */
static void ensure_sieve(idx_t n)
{
    if (n <= max_n_sieve)
        return;
    if (primes_arr)
        free(primes_arr);
    /* build_primes_upto allocates a new array and sets primes_out */
    build_primes_upto(n, &primes_arr, &primes_cnt);
    max_n_sieve = n;
}

seq_t a(idx_t n)
{
    /* Our algorithm works for any n ≥ 1.  For n < 7 all
     * representations are impossible, but the loops below
     * will automatically return 0. */
    seq_t result = 0;

    /* Spring a sieve up to n, possibly expanding the cached list. */
    ensure_sieve(n);

    /* Multiplier for p: 2 if n is even, 1 if n is odd. */
    uint64_t mul = (n & 1ULL) ? 1ULL : 2ULL;

    /* Outer loop over p (index pi). */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes_arr[pi];
        if (p > n)          /* p cannot exceed n itself           */
            break;

        /* Check primality of 2p² − 1.  For p ≤ n ≤ 10000,
         * this value fits easily in idx_t. */
        uint64_t valp = 2ULL * p * p - 1ULL;
        if (!is_prime(valp))
            continue;

        /* Remaining sum that must be split into q + r. */
        if (mul * p > n)
            continue;
        uint64_t remaining = n - mul * p;

        /* Inner loop over q (starting from pi to keep p ≤ q). */
        for (size_t qi = pi; qi < primes_cnt; ++qi) {
            uint64_t q = primes_arr[qi];

            /* q cannot exceed remaining/2, otherwise r < q. */
            if (q > remaining / 2ULL)
                break;

            uint64_t valq = 2ULL * q * q - 1ULL;
            if (!is_prime(valq))
                continue;

            uint64_t r = remaining - q;
            if (r < q)            /* enforce q ≤ r */
                continue;
            if (r > n)            /* r never exceeds n, but guard anyway */
                continue;
            if (!is_prime(r))
                continue;

            uint64_t valr = 2ULL * r * r - 1ULL;
            if (!is_prime(valr))
                continue;

            /* All six numbers are prime → valid representation. */
            ++result;
        }
    }
    return result;
}
