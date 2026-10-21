/*
 * A237413  Number of ways to write n = k + m with k > 0 and m > 0 such that
 *          p(k)^2 - 2, p(m)^2 - 2 and p(p(m))^2 - 2 are all prime, where p(j)
 *          denotes the j-th prime.
 * https://oeis.org/A237413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime, prime_limit_for_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by checking all decompositions n = k + m with k > 0, m > 0
   and verifying that p(k)^2 - 2, p(m)^2 - 2 and p(p(m))^2 - 2 are prime,
   where p(j) is the j‑th prime.  The function keeps a cached list of
   primes and expands it lazily to the size required for each call. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/prime_limit_for_idx.h"
#include "../utils/is_prime.h"

static uint64_t *g_primes = NULL;    /* array of primes, 1‑based indexing: g_primes[0] = 2 */
static size_t   g_primes_cnt = 0;    /* number of primes allocated */

/* Ensure that g_primes contains at least idx primes. */
static void
ensure_primes_up_to_idx(idx_t idx)
{
    if (g_primes_cnt < idx) {
        if (g_primes) {
            free(g_primes);
            g_primes = NULL;
            g_primes_cnt = 0;
        }
        idx_t limit = prime_limit_for_idx(idx);          /* numeric limit guaranteeing idx primes */
        build_primes_upto(limit, &g_primes, &g_primes_cnt);
    }
}

seq_t
a(idx_t n)
{
    if (n <= 1)                 /* offset 1: a(1) = 0 */
        return 0;

    /* Pre‑compute primes for indices up to n-1 */
    ensure_primes_up_to_idx(n - 1);
    /* Largest prime index needed for p(p(m)) is p_{n-1} */
    uint64_t p_max = g_primes[n - 2];   /* prime at index n-1 */
    ensure_primes_up_to_idx(p_max);

    seq_t count = 0;

    for (idx_t k = 1; k < n; ++k) {
        idx_t m = n - k;           /* m > 0 */

        uint64_t p_k = g_primes[k - 1];
        uint64_t p_m = g_primes[m - 1];
        uint64_t p_p_m = g_primes[p_m - 1];

        if (!is_prime((idx_t)p_k * p_k - 2ULL))
            continue;
        if (!is_prime((idx_t)p_m * p_m - 2ULL))
            continue;
        if (!is_prime((idx_t)p_p_m * p_p_m - 2ULL))
            continue;

        ++count;
    }

    return count;
}
