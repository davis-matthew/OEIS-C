/*
 * A239884  Least positive integer k <= n with pi(pi(k*n)) a square, or 0 if
 *          such a number k does not exist, where pi(x) denotes the number of
 *          primes not exceeding x.
 * https://oeis.org/A239884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 378, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Definition: a(n) = least k ≤ n such that π(π(k*n)) is a perfect square,
 * otherwise 0.  (π(x) = #{primes ≤ x})
 *
 * The routine builds a prime sieve up to n² lazily, and scans k = 1…n
 * looking for the first k that satisfies the condition.  All arithmetic is
 * performed in the integer types supplied by the build (idx_t for indices,
 * seq_t for the return value).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* Global sieve state */
static idx_t *prime_array = NULL;     /* ascending primes */
static size_t prime_count  = 0;        /* number of primes in prime_array */
static idx_t  sieve_limit  = 0;        /* current upper bound of the sieve */

/* Ensure that primes are available up to at least "limit". */
static void
ensure_limit(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* Replace the old prime list with a new one. */
    free(prime_array);
    build_primes(limit, &prime_array, &prime_count);
    sieve_limit = limit;
}

/* Integer square‑root for idx_t values. */
static inline idx_t
isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        if (mid * mid <= n) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

/* Number of primes ≤ x, using the global prime array. */
static inline idx_t
pi_upto(idx_t x)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if ((idx_t)prime_array[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)lo;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety: offset starts at 1 */

    /* Prepare a sieve up to n², the maximum product k*n. */
    idx_t max_needed = n * n;
    ensure_limit(max_needed);

    /* Scan k from 1 up to n, returning the first that works. */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t prod = k * n;
        idx_t pi1  = pi_upto(prod);
        idx_t pi2  = pi_upto(pi1);
        idx_t s    = isqrt_idx(pi2);
        if (s * s == pi2) {                 /* π(π(k·n)) is a square */
            return (seq_t)k;
        }
    }
    return (seq_t)0;                       /* should not happen for n ≥ 1 */
}
