/*
 * A087974  Largest value of number of distinct prime factors arising in the
 *          3x+1 iteration trajectory started with n.
 * https://oeis.org/A087974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute the maximum number of distinct prime factors
   encountered in the 3x+1 (Collatz) trajectory starting at n.
   The function uses a linear sieve up to a limit sufficient for
   n <= 10000, and computes ω(n) (number of distinct prime factors)
   for all intermediate values on the fly. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* static data for the sieve and caching */
static bool initialized = false;
static uint32_t *spf = NULL;           /* least prime factor array */
static int *omega_cache = NULL;        /* -1 = uncomputed, else ω value */
static uint32_t *primes = NULL;        /* list of primes up to LIMIT */
static size_t primes_cnt = 0;

/* maximum value we sieve to.  Enough for n <= 10000. */
#define SIEVE_LIMIT 10000000U

/* one‑time initialization of the sieve */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    const size_t LIMIT = SIEVE_LIMIT;

    spf = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));
    omega_cache = (int*)malloc((LIMIT + 1) * sizeof(int));
    for (size_t i = 0; i <= LIMIT; ++i) omega_cache[i] = -1;
    omega_cache[1] = 0;

    /* primes array large enough for the sieve */
    size_t prime_cap = LIMIT / 10 + 100;
    primes = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    primes_cnt = 0;

    /* linear sieve */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_cnt == prime_cap) {
                prime_cap *= 2;
                primes = (uint32_t*)realloc(primes, prime_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt &&
                 primes[j] <= spf[i] &&
                 (uint64_t)i * primes[j] <= LIMIT; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
}

/* ω(n) for n <= LIMIT using the sieve cache */
static int omega_small(uint32_t v)
{
    int val = omega_cache[v];
    if (val != -1) return val;
    uint32_t last = 0, x = v;
    int cnt = 0;
    while (x > 1) {
        uint32_t p = spf[x];
        if (p != last) {
            ++cnt;
            last = p;
        }
        x /= p;
    }
    omega_cache[v] = cnt;
    return cnt;
}

/* ω(n) for arbitrary n (uses the list of primes up to LIMIT) */
static int omega_u64(uint64_t v)
{
    if (v <= SIEVE_LIMIT) return omega_small((uint32_t)v);
    int cnt = 0;
    uint64_t temp = v;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > temp) break;
        if (temp % p == 0) {
            ++cnt;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) ++cnt;        /* leftover prime factor > sqrt(v) */
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init();

    uint64_t cur = (uint64_t)n;
    seq_t max_omega = 0;
    while (1) {
        seq_t o = (seq_t)omega_u64(cur);
        if (o > max_omega) max_omega = o;
        if (cur == 1) break;
        if (cur & 1ULL)
            cur = cur * 3ULL + 1ULL;
        else
            cur >>= 1ULL;
    }
    return max_omega;
}
