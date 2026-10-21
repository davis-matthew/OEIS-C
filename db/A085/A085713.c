/*
 * A085713  Consider numbers k such that phi(x) = k has exactly 3 solutions
 *          and they are (3*p, 4*p, 6*p) where p is 1 or a prime. Sequence
 *          gives values of p.
 * https://oeis.org/A085713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth term of sequence A085713 – the p such that phi(x) =
 *            2(p‑1) has exactly the three solutions (3p,4p,6p)
 * Range:     u8 n <= 20, u16 n <= 3300, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Utilities ---------- */
static const idx_t PRIME_LIMIT = 10000000;  /* upper bound for prime sieve */
static uint8_t *sieve = NULL;
static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;
static bool primes_built = false;

static void build_primes(void) {
    if (primes_built) return;
    sieve = (uint8_t*)malloc(PRIME_LIMIT + 1);
    memset(sieve, 1, PRIME_LIMIT + 1);
    sieve[0] = sieve[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;
    prime_list = (uint32_t*)malloc((PRIME_LIMIT/2 + 1000) * sizeof(uint32_t));
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) prime_list[prime_cnt++] = i;
    primes_built = true;
}

/* ---------- Solver for phi(n) = m ---------- */
static seq_t target_p;
static bool found3, found4, found6;
static size_t sol_cnt;
static bool early_stop;

static void dfs_uint64(uint64_t m_rem, size_t last_idx, uint64_t curr_n) {
    if (early_stop) return;
    if (m_rem == 1) {
        if (curr_n == 3ULL * (uint64_t)target_p) found3 = true;
        else if (curr_n == 4ULL * (uint64_t)target_p) found4 = true;
        else if (curr_n == 6ULL * (uint64_t)target_p) found6 = true;
        else { early_stop = true; return; }
        ++sol_cnt;
        if (sol_cnt > 3) early_stop = true;
        return;
    }
    for (size_t i = last_idx; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)(p - 1) > m_rem) break;
        if (m_rem % (p - 1) != 0) continue;
        uint64_t pow = 1;
        while (1) {
            uint64_t val = (uint64_t)(p - 1) * pow;
            if (m_rem % val != 0) break;
            uint64_t new_m = m_rem / val;
            __int128 tmp = (__int128)curr_n * pow * p;
            if (tmp > ( (__int128)1 << 63 ) - 1) { early_stop = true; return; }
            uint64_t new_n = (uint64_t)tmp;
            if (new_m == 1) {
                if (new_n == 3ULL * (uint64_t)target_p) found3 = true;
                else if (new_n == 4ULL * (uint64_t)target_p) found4 = true;
                else if (new_n == 6ULL * (uint64_t)target_p) found6 = true;
                else { early_stop = true; return; }
                ++sol_cnt;
                if (sol_cnt > 3) { early_stop = true; return; }
            } else {
                dfs_uint64(new_m, i + 1, new_n);
                if (early_stop) return;
            }
            if (pow > m_rem / p) break;
            pow *= p;
        }
    }
}

/* ---------- Sequence storage ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static size_t prime_pos = 0;   /* next prime index to test */

static void cache_append(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? 2 * cache_cap : 16;
        seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- Main function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but guard */
    build_primes();
    /* Ensure cache has at least n terms */
    while (cache_len < (size_t)n) {
        if (cache_len == 0) {             /* first term is 1 */
            cache_append((seq_t)1);
            prime_pos = 0;
            continue;
        }
        /* Find next prime that satisfies the condition */
        while (1) {
            if (prime_pos >= prime_cnt) {
                /* Should not happen for required n */
                return 0;
            }
            uint32_t p = prime_list[prime_pos++];
            if (p == 2 || p == 3) continue;   /* excluded automatically */
            target_p = (seq_t)p;
            uint64_t k = 2ULL * ((uint64_t)p - 1ULL);
            found3 = found4 = found6 = false;
            sol_cnt = 0;
            early_stop = false;
            dfs_uint64(k, 0, 1ULL);
            if (!early_stop && found3 && found4 && found6 && sol_cnt == 3) {
                cache_append((seq_t)p);
                break;
            }
        }
    }
    return cache[n - 1];
}
