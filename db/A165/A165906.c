/*
 * A165906  Sum of first n primes divided by the n-th prime, rounded up to the
 *          nearest integer.
 * https://oeis.org/A165906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceiling((∑_{k=1..n} prime(k)) / prime(n))
 * Range:     u8 n <= 13, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Static cache for primes and their cumulative sums */
static idx_t *primes = NULL;          /* array of primes (idx_t) */
static size_t primes_cap = 0;         /* current capacity of primes array */
static size_t primes_cnt = 0;         /* number of primes stored */

static seq_t *cumsums = NULL;        /* cumulative sums of primes (seq_t) */
static size_t cumsums_cap = 0;       /* current capacity of cumsums array */

/* Check if a candidate number is prime using previously found primes */
static int is_prime_candidate(idx_t cand)
{
    if (cand < 2u) return 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        /* Stop if p*p > cand (use 128‑bit to avoid overflow) */
        if ((unsigned __int128)p * p > (unsigned __int128)cand) break;
        if (cand % p == 0u) return 0;
    }
    return 1;
}

/* Append the next prime to the cache, updating the cumulative sums */
static void append_next_prime(void)
{
    idx_t cand = primes_cnt ? primes[primes_cnt - 1] + 1u : 2u;
    if (cand > 2u && (cand & 1u) == 0u) cand += 1u; /* make it odd unless 2 */

    /* Find the next prime */
    for (;;) {
        if (is_prime_candidate(cand)) break;
        cand += 2u;
        if (cand == 2u) cand = 3u; /* safety */
    }

    /* Grow primes array if necessary */
    if (primes_cnt == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 4ull;
        primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        primes_cap = new_cap;
    }
    primes[primes_cnt] = cand;

    /* Compute cumulative sum up to this prime */
    seq_t prev_sum = primes_cnt ? cumsums[primes_cnt - 1] : (seq_t)0;
    seq_t new_sum = prev_sum + (seq_t)cand;

    /* Grow cumsums array if necessary */
    if (primes_cnt == cumsums_cap) {
        size_t new_cap = cumsums_cap ? cumsums_cap << 1 : 4ull;
        cumsums = (seq_t *)realloc(cumsums, new_cap * sizeof(seq_t));
        cumsums_cap = new_cap;
    }
    cumsums[primes_cnt] = new_sum;

    ++primes_cnt;
}

/* Ensure that at least n primes are stored in the cache */
static void ensure_primes(idx_t n)
{
    while (primes_cnt < n) {
        append_next_prime();
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0; /* value outside offset not relevant */

    ensure_primes(n);

    seq_t sum   = cumsums[n - 1];
    seq_t prime = (seq_t)primes[n - 1];
    /* ceil division: (sum + prime – 1) / prime */
    seq_t res = (sum + prime - (seq_t)1) / prime;
    return res;
}
