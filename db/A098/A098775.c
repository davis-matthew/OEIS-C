/*
 * A098775  Palindromic abundant numbers.
 * https://oeis.org/A098775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A098775 – palindromic abundant numbers.  
    The sequence is produced by generating all decimal palindromes in
    increasing order and keeping those that are abundant (sum of proper
    divisors > number).  The nth term is returned by a(n).  The
    implementation caches the found numbers so that later requests are
    answered quickly.  The approach is O(N) in the index n and uses
    simple trial‑division with a precomputed prime table. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_palindrome.h"

/* Powers of ten up to 10^19 – enough for the required palindrome lengths. */
static const uint64_t pow10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL,
    100000ULL, 1000000ULL, 10000000ULL, 100000000ULL,
    1000000000ULL, 10000000000ULL, 100000000000ULL,
    1000000000000ULL, 10000000000000ULL, 100000000000000ULL,
    1000000000000000ULL, 10000000000000000ULL,
    100000000000000000ULL, 1000000000000000000ULL, 10000000000000000000ULL
};

/* ---------- Prime table (up to 1,000,000) ---------- */
#define PRIME_LIMIT 1000000
static uint32_t primes[78498];   /* 78498 primes <= 1,000,000 */
static size_t prime_cnt = 0;

static void build_primes(void)
{
    if (prime_cnt != 0) return;                 /* already built */
    bool *is_composite = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    size_t pos = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[pos++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    prime_cnt = pos;
    free(is_composite);
}

/* ---------- Abundant check (using uint64_t factorisation) ---------- */
static bool is_abundant(uint64_t n)
{
    uint64_t sigma = 1;        /* product of (p^(k+1)-1)/(p-1) for each prime power */
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            uint64_t term = 1;
            uint64_t p_pow = 1;
            while (m % p == 0) {
                m /= p;
                p_pow *= p;
                term += p_pow;
            }
            sigma *= term;
            if (sigma > 2ULL * n) return true;   /* early exit */
        }
    }
    if (m > 1) {                       /* remaining prime factor */
        sigma *= (1ULL + m);
        if (sigma > 2ULL * n) return true;
    }
    return sigma > 2ULL * n;
}

/* ---------- Cache structure for found numbers ---------- */
static seq_t *pal_abund_cache = NULL;
static size_t pal_abund_len = 0;
static size_t pal_abund_cap = 0;

static void cache_append(seq_t val)
{
    if (pal_abund_len == pal_abund_cap) {
        size_t new_cap = (pal_abund_cap == 0) ? 16 : pal_abund_cap << 1;
        seq_t *tmp = (seq_t *)realloc(pal_abund_cache, new_cap * sizeof(seq_t));
        if (!tmp) return;               /* allocation failure – unlikely */
        pal_abund_cache = tmp;
        pal_abund_cap = new_cap;
    }
    pal_abund_cache[pal_abund_len++] = val;
}

/* ---------- Generate palindromes until target count reached ---------- */
typedef struct {
    uint32_t cur_len;        /* current palindrome length */
    uint32_t half_len;       /* (cur_len+1)/2 */
    uint32_t half_start;     /* first half value for this length */
    uint32_t half_limit;     /* last half value for this length */
    uint32_t current_half;   /* next half to use */
    bool     initialised;    /* indicates whether the static state is set */
} gen_state_t;

static gen_state_t gs = {0,0,0,0,0,false};

static void generate_until(size_t target)
{
    if (gs.initialised == false) {
        gs.cur_len = 2;
        gs.half_len = 1;
        gs.half_start = (uint32_t)pow10[gs.half_len - 1];
        gs.half_limit = (uint32_t)pow10[gs.half_len] - 1;
        gs.current_half = gs.half_start;
        gs.initialised = true;
    }

    build_primes();     /* ensures prime list is ready */

    while (pal_abund_len < target) {
        uint32_t half = gs.current_half++;
        uint64_t pal = build_palindrome(half, gs.cur_len);
        if (is_abundant(pal)) {
            cache_append((seq_t)pal);
        }

        if (gs.current_half > gs.half_limit) {
            /* advance to next length */
            gs.cur_len++;
            gs.half_len = (gs.cur_len + 1) / 2;
            gs.half_start = (uint32_t)pow10[gs.half_len - 1];
            gs.half_limit = (uint32_t)pow10[gs.half_len] - 1;
            gs.current_half = gs.half_start;
        }
    }
}

/* ---------- Public interface required by the template ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but safe */
    generate_until((size_t)n);
    return pal_abund_cache[n - 1];
}
