/*
 * A116997  Length of chain starting 2n, iterating f(m) = m - (number of
 *          distinct representations of m as the sum of two primes).
 * https://oeis.org/A116997
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of steps in the chain starting at 2n and *
 *            iterating m → m – g(m) until m becomes odd, where g(m) is the *
 *            number of distinct unordered representations of m as a sum of
 *            two primes. * * This implementation builds a prime sieve up to
 *            2·n on demand, * precomputes the Goldbach counts g(...
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ---------- 1. Static storage for primes and Goldbach counts ---------- */
static uint64_t *prime_list = NULL;     /* array of primes up to current limit   */
static size_t    prime_cnt  = 0;        /* number of primes in prime_list        */

static bool *is_prime   = NULL;        /* primality table for indices 0..limit   */
static size_t is_prime_cap = 0;         /* allocated size of is_prime            */

static seq_t *gcnt   = NULL;           /* Goldbach counts for each m            */
static size_t gcnt_cap = 0;             /* allocated size of gcnt                */

static size_t  max_limit = 0;          /* maximum value of 2·n already processed */

/* ---------- 2. Helper to extend data structures up to new_limit ---------- */
static void
ensure_up_to(idx_t n)
{
    size_t needed_lim = (size_t)2 * (size_t)n;
    if (needed_lim <= max_limit)
        return;                       /* nothing to be done */

    /* -- sieve primes up to needed_lim --------------------------------- */
    bool *new_is_prime = (bool *)malloc((needed_lim + 1) * sizeof(bool));
    if (!new_is_prime) return;        /* out‑of‑memory – not expected in tests */

    for (size_t i = 0; i <= needed_lim; ++i)
        new_is_prime[i] = true;        /* initialise to prime */
    if (needed_lim >= 0) new_is_prime[0] = false;
    if (needed_lim >= 1) new_is_prime[1] = false;

    size_t sqrt_lim = (size_t)sqrt((double)needed_lim);
    for (size_t p = 2; p <= sqrt_lim; ++p) {
        if (!new_is_prime[p]) continue;
        for (size_t j = p * p; j <= needed_lim; j += p)
            new_is_prime[j] = false;
    }

    /* collect prime list */
    size_t new_prime_cnt = 0;
    for (size_t i = 2; i <= needed_lim; ++i)
        if (new_is_prime[i]) ++new_prime_cnt;

    uint64_t *new_prime_list = (uint64_t *)malloc(new_prime_cnt * sizeof(uint64_t));
    if (!new_prime_list) return;      /* out‑of‑memory – not expected */
    size_t idx = 0;
    for (size_t i = 2; i <= needed_lim; ++i)
        if (new_is_prime[i]) new_prime_list[idx++] = (uint64_t)i;

    /* -- goldbach counts for all even m up to needed_lim ----------------- */
    seq_t *new_gcnt = (seq_t *)malloc((needed_lim + 1) * sizeof(seq_t));
    if (!new_gcnt) return;
    for (size_t i = 0; i <= needed_lim; ++i)
        new_gcnt[i] = 0;

    /* count unordered prime pairs.  For each pair (p_i, p_j) with i≤j,
     * increase gcnt[p_i + p_j] by one if the sum fits. */
    for (size_t i = 0; i < new_prime_cnt; ++i) {
        uint64_t pi = new_prime_list[i];
        for (size_t j = i; j < new_prime_cnt; ++j) {
            uint64_t pj = new_prime_list[j];
            uint64_t sum = pi + pj;
            if (sum > needed_lim) break;
            ++new_gcnt[(size_t)sum];
        }
    }

    /* ---- free old data and replace with new ---- */
    free(is_prime);
    free(prime_list);
    free(gcnt);

    is_prime      = new_is_prime;
    is_prime_cap  = needed_lim + 1;
    prime_list    = new_prime_list;
    prime_cnt     = new_prime_cnt;
    gcnt          = new_gcnt;
    gcnt_cap      = needed_lim + 1;
    max_limit     = needed_lim;
}

/* ---------- 3. Main sequence function ---------------------------------- */
seq_t
a(idx_t n)
{
    if (n < 2) return 0;             /* offset region – unspecified behaviour */

    ensure_up_to(n);

    seq_t steps  = 1;                /* count the starting value 2n   */
    idx_t m      = (idx_t)2 * n;     /* current number in the chain   */

    while ((m & 1ULL) == 0) {        /* continue while m is even       */
        seq_t cnt = gcnt[(size_t)m]; /* number of unordered prime pairs*/
        if (cnt == 0) break;         /* should not happen for even m≥4  */
        m -= (idx_t)cnt;
        ++steps;
    }
    return steps;
}
