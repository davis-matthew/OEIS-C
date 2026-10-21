/*
 * A092780  Sum(prime(k),k=1..n)^2-1.
 * https://oeis.org/A092780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of the first n primes)^2 - 1 * = A007504(n)^2 - 1 *
 *            * The function computes the formula exactly for any n ≥ 1, *
 *            memoizing the primes and their prefix sums. * It uses a simple
 *            sieve to generate the needed primes up to an * estimated upper
 *            bound for the n‑th prime
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Cache for primes and their prefix sums */
static uint64_t *prime_cache   = NULL;   /* array of primes */
static seq_t   *sum_prefix    = NULL;   /* prefix sums of primes */
static size_t  prime_count    = 0;       /* number of primes stored */

/* ------------------------------------------------------------------
 * Helper: estimate an upper bound for the n‑th prime.
 * ------------------------------------------------------------------ */
static inline idx_t prime_limit_for_idx(idx_t n)
{
    if (n < 6)
        return 15;                     /* safe upper bound for small n */

    double dn   = (double)n;
    double lim  = dn * (log(dn) + log(log(dn))) + 3.0;  /* Rosser's bound */
    return (idx_t)lim;
}

/* ------------------------------------------------------------------
 * Sieve of Eratosthenes: generate all primes ≤ limit,
 * store them in prime_cache and their prefix sums in sum_prefix.
 * This function rebuilds the caches from scratch if `n` exceeds
 * the current number of stored primes.
 * ------------------------------------------------------------------ */
static void ensure_primes(idx_t n)
{
    if (n <= prime_count)
        return;                                 /* enough primes already */

    idx_t limit = prime_limit_for_idx(n);

    size_t sz   = (size_t)limit + 1;
    bool *is_composite = (bool*)calloc(sz, sizeof(bool));
    if (!is_composite)
        abort();      /* out of memory */

    /* Classic sieve */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_composite[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;

    /* Count primes until we reach at least n */
    size_t count = 0;
    for (idx_t i = 2; i <= limit && count < n; ++i)
        if (!is_composite[i]) ++count;

    /* In the extremely rare case the bound was too small, retry */
    if (count < n) {
        free(is_composite);
        ensure_primes(n + (n / 2 + 10)); /* enlarge bound and rebuild */
        return;
    }

    /* Allocate new arrays */
    uint64_t *new_primes = (uint64_t*)malloc(count * sizeof(uint64_t));
    seq_t    *new_sums   = (seq_t*)malloc(count * sizeof(seq_t));
    if (!new_primes || !new_sums) {
        free(is_composite);
        abort();
    }

    /* Fill arrays and compute prefix sums */
    seq_t acc = 0;
    size_t idx = 0;
    for (idx_t i = 2; i <= limit && idx < count; ++i) {
        if (!is_composite[i]) {
            new_primes[idx] = (uint64_t)i;
            acc += (seq_t)i;
            new_sums[idx]  = acc;
            ++idx;
        }
    }

    free(is_composite);

    /* Replace old caches with the new ones */
    free(prime_cache);
    free(sum_prefix);
    prime_cache   = new_primes;
    sum_prefix    = new_sums;
    prime_count   = count;
}

/* ------------------------------------------------------------------
 * Return the sum of the first n primes (A007504(n)).
 * ------------------------------------------------------------------ */
static seq_t sum_first_n_primes(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_primes(n);
    return sum_prefix[n - 1];
}

/* ------------------------------------------------------------------
 * Main sequence function required by the specification.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 */
        return 0;

    seq_t s   = sum_first_n_primes(n);
    seq_t sq  = s * s;          /* use seq_t arithmetic */
    return sq - 1;
}
