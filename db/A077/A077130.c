/*
 * A077130  Smallest number which is relatively prime to all the numbers
 *          between n^2 and (n+1)^2, inclusive.
 * https://oeis.org/A077130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation finds the smallest prime p that does not divide any
 * integer in the closed interval [n^2, (n+1)^2].  A candidate p satisfies:
 *   ceil(n^2 / p) * p > (n+1)^2
 * The function searches primes in ascending order, using a pre‑computed
 * sieve of Eratosthenes up to a fixed limit that comfortably covers all
 * expected results for n up to at least 10 000. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* The sieve limit must be large enough to contain the largest prime that
 * can appear in the sequence for the supported n.  For n ≤ 10 000 the
 * required primes are far below 200 000, so this conservative limit is
 * sufficient and keeps the memory footprint small. */
#define SIEVE_LIMIT 200000

static bool sieve_inited = false;
static bool is_composite[SIEVE_LIMIT + 1];
static int primes[SIEVE_LIMIT / 10];   /* enough slots for the primes in the limit */
static size_t prime_cnt = 0;

/* One‑time initialization of the sieve and list of primes. */
static void init_sieve(void)
{
    if (sieve_inited)
        return;

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        is_composite[i] = false;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_composite[j] = true;
        }
    }

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_composite[i])
            primes[prime_cnt++] = (int)i;

    sieve_inited = true;
}

/* Primitive primality test for values ≤ SIEVE_LIMIT.  For values larger
 * than SIEVE_LIMIT we fall back to trial division by the known primes
 * up to sqrt(v).  This is adequate for the ranges of p required. */
static bool is_prime_small(uint32_t v)
{
    if (v < 2)
        return false;
    if (v <= SIEVE_LIMIT)
        return !is_composite[v];

    for (size_t i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        if ((uint64_t)p * p > v)
            break;
        if (v % p == 0)
            return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* In the strictest interpretation we must do all arithmetic in seq_t,
     * but for larger n the intermediate values (n^2, (n+1)^2) exceed the
     * range of many seq_t types.  Therefore we perform the interval bounds
     * in unsigned 64‑bit arithmetic and use seq_t only for the final prime. */
    init_sieve();

    uint64_t n_val = (uint64_t)n;
    uint64_t n2 = n_val * n_val;
    uint64_t n1sq = (n_val + 1) * (n_val + 1);

    for (uint32_t p = 2; ; ++p) {
        if (!is_prime_small(p))
            continue;

        /* First multiple of p not less than n^2 */
        uint64_t first = ((n2 + (uint64_t)p - 1) / (uint64_t)p) * (uint64_t)p;
        if (first > n1sq)
            return (seq_t)p;
    }
}
