/*
 * A174163  a(n) = (A173919(k)*A173919(k+1))/2.
 * https://oeis.org/A174163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A173919(k)*A173919(k+1))/2 where A173919(k) = the k‑th
 *            integer m such that m or m+1 is prime
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- Prime helpers ----------------------------------------------------- */
static uint32_t *prime_list  = NULL;
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;
static bool     primes_ok   = false;

static void init_primes(void)
{
    if (primes_ok) return;
    const uint32_t LIMIT = 200000U;            /* safe bound for n≈10 000 terms */
    bool *sieve = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!sieve) abort();
    for (size_t i = 0; i <= LIMIT; ++i) sieve[i] = false;

    prime_cap = 20000U;
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) { free(sieve); abort(); }

    prime_cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            if (prime_cnt >= prime_cap) {
                prime_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list,
                                                    prime_cap * sizeof(uint32_t));
                if (!tmp) { free(sieve); abort(); }
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i) sieve[j] = true;
            }
        }
    }
    free(sieve);
    primes_ok = true;
}

static bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt &&
           (uint64_t)prime_list[i] * prime_list[i] <= n; ++i) {
        if (n % prime_list[i] == 0) return false;
    }
    return true;
}

/* ---- A173919 cache ---------------------------------------------------- */
static seq_t *t_cache = NULL;
static size_t t_cap   = 0;
static size_t t_cnt   = 0;
static idx_t  next_candidate = 1;

static void grow_t_cache(void)
{
    size_t new_cap = t_cap ? t_cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(t_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    t_cache = tmp;
    t_cap   = new_cap;
}

static void ensure_t(size_t required)
{
    init_primes();
    while (t_cnt < required) {
        uint64_t m = (uint64_t)next_candidate;
        bool ok = is_prime_small(m) || is_prime_small(m + 1);
        if (ok) {
            if (t_cnt >= t_cap) grow_t_cache();
            t_cache[t_cnt++] = (seq_t)m;
        }
        ++next_candidate;
    }
}

/* ---- public sequence function ------------------------------------------ */
/* Formula from OEIS: a(n) = (A173919(k)*A173919(k+1))/2. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                        /* out of offset domain */
    ensure_t((size_t)n + 1);                     /* need T[n] and T[n+1] */
    seq_t v1 = t_cache[n - 1];
    seq_t v2 = t_cache[n];
    return (v1 * v2) / 2;
}
