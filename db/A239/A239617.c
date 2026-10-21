/*
 * A239617  Number of ways to write 2*n = p + q with p, q and pi(2*p) - pi(p)
 *          all prime, where pi(x) denotes the number of primes not exceeding
 *          x.
 * https://oeis.org/A239617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways to write 2*n = p + q with p, q prime and
   pi(2*p) - pi(p) prime.  The count is over ordered pairs (p,q). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime sieve infrastructure                                         */
/* ------------------------------------------------------------------ */

static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static idx_t sieve_limit = 0;

static void rebuild_sieve(idx_t limit)
{
    bool *is_prime = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_prime) return;

    for (size_t i = 2; i <= (size_t)limit; ++i)
        is_prime[i] = true;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t k = p * p; k <= limit; k += p)
                is_prime[k] = false;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *new_list = (uint64_t*)realloc(prime_list, cnt * sizeof(uint64_t));
    if (!new_list) {
        free(is_prime);
        return;
    }
    prime_list = new_list;
    prime_cnt = cnt;

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint64_t)i;

    free(is_prime);
    sieve_limit = limit;
}

static void ensure_limit(idx_t limit)
{
    if (limit <= sieve_limit) return;
    rebuild_sieve(limit);
}

static size_t count_primes_upto(idx_t x)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/* Small prime test (works for numbers comfortably below 2^64)        */
/* ------------------------------------------------------------------ */
static bool is_prime_small(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t limit = 4 * n;      /* enough to compute pi(2p) and pi(2q) */
    ensure_limit(limit);

    seq_t ways = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > 2 * n) break;

        idx_t q = 2 * n - p;
        if (q < 2) continue;          /* q must be prime */

        /* check q is prime */
        size_t pi_q = count_primes_upto(q);
        if (pi_q == 0 || prime_list[pi_q - 1] != q) continue;

        /* check pi(2p) - pi(p) is prime */
        size_t pi_2p = count_primes_upto((idx_t)(p << 1));
        size_t pi_p  = count_primes_upto(p);
        uint64_t diff = pi_2p - pi_p;
        if (is_prime_small(diff))
            ++ways;
    }

    return ways;
}
