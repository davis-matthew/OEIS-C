/*
 * A053670  Least number coprime to n and n+1.
 * https://oeis.org/A053670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = least integer >1 coprime to both n and n+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple sieve to pre‑compute a list of primes
   sufficient for n up to at least 10⁶ (practically the tests only
   require n≈10⁴).  For each query it factors n and n+1, collects
   their distinct prime factors, and then scans the prime list to
   find the smallest prime that is not in either factor set.  That
   prime is the required value.  The algorithm is O(π(√n)) per call
   which is <O(n) for the expected range. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool primes_inited = false;
static unsigned *primes = NULL;
static size_t primes_cnt = 0;

/* Simple sieve to generate primes up to LIMIT.  LIMIT is chosen
   large enough for the test ranges. */
static void init_primes(void)
{
    if (primes_inited) return;
    const size_t LIMIT = 200000;          /* > sqrt(10⁴) * 10, enough for safety */
    bool *is_composite = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_composite) return;            /* unlikely, but keep build clean */
    for (size_t i = 0; i <= LIMIT; ++i) is_composite[i] = false;
    size_t cnt = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) is_composite[j] = true;
        }
    }
    for (size_t i = 2; i <= LIMIT; ++i) if (!is_composite[i]) ++cnt;
    primes = (unsigned*)malloc(cnt * sizeof(unsigned));
    if (!primes) { free(is_composite); return; }
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (!is_composite[i]) primes[idx++] = (unsigned)i;
    primes_cnt = cnt;
    free(is_composite);
    primes_inited = true;
}

/* Factor the integer 'x' into its distinct prime factors,
   storing them in the array 'fac' (which has size at least 10).
   Returns the number of distinct primes found. */
static size_t distinct_prime_factors(uint64_t x, unsigned fac[10])
{
    size_t cnt = 0;
    if (x <= 1) return cnt;
    size_t i = 0;
    while (i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= x) {
        if (x % primes[i] == 0) {
            fac[cnt++] = primes[i];
            while (x % primes[i] == 0) x /= primes[i];
        }
        ++i;
    }
    if (x > 1 && cnt < 10) fac[cnt++] = (unsigned)x;    /* x itself is prime */
    return cnt;
}

seq_t a(idx_t n)
{
    init_primes();

    /* Collect distinct prime factors of n and n+1 */
    unsigned fac1[10], fac2[10];
    size_t c1 = distinct_prime_factors((uint64_t)n, fac1);
    size_t c2 = distinct_prime_factors((uint64_t)(n + 1), fac2);

    /* Search for the smallest prime not in either set */
    for (size_t i = 0; i < primes_cnt; ++i) {
        unsigned p = primes[i];
        bool in_set = false;
        for (size_t j = 0; j < c1 && !in_set; ++j) if (p == fac1[j]) in_set = true;
        for (size_t j = 0; j < c2 && !in_set; ++j) if (p == fac2[j]) in_set = true;
        if (!in_set) return (seq_t)p;
    }

    /* If we run out of pre‑generated primes (unlikely), continue searching */
    unsigned p = primes[primes_cnt - 1];
    while (true) {
        ++p;
        if (p <= 1) continue;
        bool is_prime = true;
        if (p > 4) {
            size_t limit = (size_t)(p == 2 || p == 3 ? p : 0); /* dummy */
            for (size_t q = 2; (uint64_t)q * (uint64_t)q <= p; ++q) {
                if (p % q == 0) { is_prime = false; break; }
            }
        }
        if (!is_prime) continue;
        bool in_set = false;
        for (size_t j = 0; j < c1 && !in_set; ++j) if (p == fac1[j]) in_set = true;
        for (size_t j = 0; j < c2 && !in_set; ++j) if (p == fac2[j]) in_set = true;
        if (!in_set) return (seq_t)p;
    }
}
