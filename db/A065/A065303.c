/*
 * A065303  Neither k nor sigma(k) is squarefree.
 * https://oeis.org/A065303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Custom generation of A065303: numbers k such that k and σ(k) are
 * not squarefree.  We lazily search integers in increasing order,
 * storing the terms that satisfy the condition in a static cache.
 * Square‑free test and divisor‑sum σ(k) are computed by prime
 * factorisation using a precomputed table of primes up to 200 000.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Prime sieve ---------------------------------------------------- */
#define PRIME_MAX 200000
static uint32_t primes[PRIME_MAX / 10];  /* enough for the sieve */
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_cnt != 0) return;            /* already initialised */

    static bool marked[PRIME_MAX + 1] = {0};
    for (uint32_t i = 2; i <= PRIME_MAX; ++i) {
        if (!marked[i]) {
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_MAX) {
                for (uint32_t j = i * i; j <= PRIME_MAX; j += i)
                    marked[j] = true;
            }
        }
    }
}

/* --- Square‑free test for uint64_t ---------------------------------- */
static bool is_squarefree_u64(uint64_t n)
{
    if (n <= 1) return true;                /* 1 is square‑free */
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        uint32_t cnt = 0;
        while (n % p == 0) {
            n /= p;
            ++cnt;
            if (cnt >= 2) return false;     /* found p² |
                                            * divide further for other primes
                                            */
        }
    }
    /* remaining factor > 1 is a prime with exponent 1 */
    return true;
}

/* --- Divisor sum σ(n) ------------------------------------------------ */
static uint64_t sigma_u64(uint64_t n)
{
    uint64_t result = 1;
    uint64_t m = n;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            uint32_t exp = 0;
            uint64_t pow = 1;
            while (m % p == 0) {
                m /= p;
                ++exp;
                pow *= p;                    /* now pow = p^exp */
            }
            /* term = (p^(exp+1) - 1) / (p - 1) = (pow * p - 1) / (p - 1) */
            uint64_t term = (pow * p - 1) / (p - 1);
            result *= term;
        }
    }
    if (m > 1) {                              /* remaining prime factor */
        /* exponent 1 -> term = m + 1 */
        result *= (m + 1);
    }
    return result;
}

/* --- Cache for sequence values ------------------------------------- */
#define CACHE_MAX 500000
static seq_t cache[CACHE_MAX];
static size_t cache_len = 0;
static uint64_t next_candidate = 1;

/* --- Main sequence function ----------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset 1: undefined for 0 */

    init_primes();                             /* one‑time prime sieve */

    /* generate terms until we have reached the requested index */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate++;

        if (!is_squarefree_u64(cand)) {
            uint64_t sigma = sigma_u64(cand);

            if (!is_squarefree_u64(sigma)) {
                if (cache_len < CACHE_MAX) {
                    cache[cache_len++] = (seq_t)cand;
                } else {
                    /* shouldn't happen for the intended input range */
                }
            }
        }
    }
    return cache[n - 1];
}
