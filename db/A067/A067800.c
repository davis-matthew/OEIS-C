/*
 * A067800  Nonprime numbers k such that phi(k) > k/2.
 * https://oeis.org/A067800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth positive integer k that is not prime and
   satisfies φ(k) > k/2                                    */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* -------------------------------------------------------------------------
   Prime generation (simple sieve up to LIMIT)
   ------------------------------------------------------------------------- */
static uint32_t *prime_list = NULL;
static size_t       prime_cnt = 0;

static void generate_primes(void)
{
    if (prime_list != NULL) return;          /* already done */

    const uint32_t LIMIT = 1000000;
    uint8_t *is_comp = (uint8_t*)malloc((size_t)LIMIT + 1);
    if (!is_comp) return;                    /* allocation failure */

    for (uint32_t i = 0; i <= LIMIT; ++i) is_comp[i] = 0;

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (is_comp[i]) continue;
        for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
            is_comp[(size_t)j] = 1;
    }

    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++cnt;

    prime_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) { free(is_comp); return; }

    size_t pos = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) prime_list[pos++] = i;

    prime_cnt = cnt;
    free(is_comp);
}

/* -------------------------------------------------------------------------
   Totient function for n ≤ 64‑bit
   ------------------------------------------------------------------------- */
static seq_t phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t res = n;
    uint64_t tmp = n;

    for (size_t i = 0; i < prime_cnt &&
                 (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            res -= res / p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) res -= res / tmp;
    return (seq_t)res;
}

/* -------------------------------------------------------------------------
   Primality test using the generated prime list
   ------------------------------------------------------------------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return n == p;
    }
    return true;
}

/* -------------------------------------------------------------------------
   Cache to store computed terms
   ------------------------------------------------------------------------- */
static seq_t  *cache        = NULL;
static size_t  cache_size   = 0;
static size_t  cache_cap    = 0;
static uint64_t next_candidate = 1;       /* next integer to test */

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                     /* ignore allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* -------------------------------------------------------------------------
   Public function
   ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool primes_inited = false;
    if (!primes_inited) {
        generate_primes();
        primes_inited = true;
    }

    if (n == 0) return (seq_t)0;          /* undefined, but harmless */

    while (cache_size < (size_t)n) {
        uint64_t m = next_candidate++;
        if (!is_prime_uint64(m)) {
            seq_t phi  = phi_uint64(m);
            seq_t half = (seq_t)(m / 2);
            if (phi > half) {
                ensure_cache_capacity(cache_size + 1);
                cache[cache_size++] = (seq_t)m;
            }
        }
    }
    return cache[(size_t)n - 1];
}
