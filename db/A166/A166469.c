/*
 * A166469  Number of divisors of n which are not multiples of consecutive
 *          primes.
 * https://oeis.org/A166469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are not multiples of two *
 *            consecutive primes. Computation by dynamic programming over the
 *            * prime factorisation of n: for each prime we may either use it
 *            (choose * a positive exponent) or skip it. Two primes that are
 *            consecutive on * the prime number line cann...
 * Range:     u8 n <= 9868, u16 n <= 9868, u32 n <= 9868, u64 n <= 9868,
 *            u128 n <= 9868, big any n
 * Helpers:   build_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes.h"
#include "../utils/factor_u64.h"

static idx_t *global_primes = NULL;
static size_t global_cnt   = 0;
static idx_t  global_limit = 0;

/* Build (or rebuild) the prime table up to at least 'limit'. */
static void ensure_global_primes(idx_t limit)
{
    if (global_primes == NULL || limit > global_limit) {
        if (global_primes != NULL) {
            free(global_primes);
            global_primes = NULL;
        }
        build_primes(limit, &global_primes, &global_cnt);
        global_limit = limit;
    }
}

/* Linear search for the index of p in the global prime table. */
static int index_in_global_primes(uint64_t p)
{
    for (size_t i = 0; i < global_cnt; ++i)
        if (global_primes[i] == p)
            return (int)i;
    return -1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; below offset is ignored */

    /* Ensure prime table is up to date. */
    ensure_global_primes(n);

    /* Factorise n.  The buffer size 64 is more than enough for 64‑bit n. */
    uint64_t fpr[64], fex[64];
    size_t  kc = 0;
    factor_u64((uint64_t)n, fpr, fex, &kc);

    /* DP variables.  dp0 = ways ending with previous prime *not* used;
     * dp1 = ways ending with previous prime used. */
    seq_t dp0 = 1;
    seq_t dp1 = 0;

    for (size_t i = 0; i < kc; ++i) {
        seq_t opts_chosen = (seq_t)fex[i]; /* e_i possibilities (exponent > 0) */
        seq_t opts_not    = 1;             /* choosing exponent 0 */

        bool adj = false;
        if (i > 0) {
            int idx_prev = index_in_global_primes(fpr[i - 1]);
            int idx_cur  = index_in_global_primes(fpr[i]);
            if (idx_prev >= 0 && idx_cur >= 0 && (size_t)idx_prev + 1 == (size_t)idx_cur)
                adj = true;
        }

        seq_t new_dp1 = opts_chosen * (dp0 + (adj ? 0 : dp1));
        seq_t new_dp0 = opts_not * (dp0 + dp1);

        dp0 = new_dp0;
        dp1 = new_dp1;
    }

    return dp0 + dp1;
}
