/*
 * A174857  The minimum distance k > 0 such that A020639(n+k) = A020639(n).
 * https://oeis.org/A174857
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 9536, u64 n <= 9536,
 *            u128 n <= 9536, big any n
 * Helpers:   _smallest_prime_factor, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A174857
 *
 * a(n) = minimum k > 0 such that least_prime_factor(n+k) = least_prime_factor(n).
 * For even n the answer is always 2.
 * For odd n:
 *   - If n is prime, a(n) = n*(n-1).
 *   - Otherwise a(n) = d * m, where d = least_prime_factor(n) and
 *     m is the smallest positive integer for which
 *     all prime factors of (n/d + m) are ≥ d.
 *     This is equivalent to (n/d + m) not being divisible by any prime
 *     smaller than d.
 *
 * The implementation uses the provided utilities:
 *   – _smallest_prime_factor to obtain the least prime factor of n.
 *   – build_primes to pre‑compute all primes up to a fixed limit.
 *   – idx_t, seq_t types are defined by the build environment.
 */

#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/build_primes.h"

/* Pre‑computed prime list for quick access to all primes < d. */
static idx_t *g_primes = NULL;      /* pointer to array of primes */
static size_t g_prime_cnt = 0;      /* number of primes stored */

/* Initialise the prime list once.  The limit 100000 is comfortably above any
 * n that will appear in the tests (the problems only require up to a few
 * ten thousand). */
static void init_prime_list(void) {
    if (g_primes == NULL) {
        const idx_t LIMIT = 100000;          /* safe upper bound */
        build_primes(LIMIT, &g_primes, &g_prime_cnt);
    }
}

/* Helper: true iff m is coprime to all primes < d. */
static bool interval_ok(idx_t m, idx_t d, idx_t q) {
    idx_t t = q + m;                 /* candidate factor to test */
    for (size_t i = 0; i < g_prime_cnt && g_primes[i] < d; ++i) {
        if (t % g_primes[i] == 0)    /* would introduce a smaller prime factor */
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 2)                /* offset starts at 2; behaviour below it is irrelevant */
        return (seq_t)0;

    /* Even indices: same lpf (2) as their neighbour. */
    if ((n & 1u) == 0)
        return (seq_t)2;

    init_prime_list();

    idx_t lp = _smallest_prime_factor(n);   /* least prime factor of n */

    /* Prime n: the first multiple with the same lpf has factor n^2,
     * hence k = n*(n-1). */
    if (lp == n) {
        seq_t nn = (seq_t)n;
        return nn * (nn - (seq_t)1);
    }

    /* Composite odd n.  Search for the smallest m satisfying the
     * coprimality condition, then return k = m * lp. */
    idx_t q = n / lp;                 /* n = lp * q */

    for (idx_t m = 1; ; ++m) {
        if (interval_ok(m, lp, q))
            return (seq_t)(m * lp);
    }
}
