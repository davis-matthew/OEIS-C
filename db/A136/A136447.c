/*
 * A136447  Numbers which are not pseudoperfect: Complement of A005835.
 * https://oeis.org/A136447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th non‑pseudoperfect number.
   a(n) is the n‑th positive integer that is not pseudoperfect
   (i.e., no subset of its proper divisors sums to the integer).
   The function searches integers in increasing order, tests
   them for pseudoperfectness using a subset–sum bitset DP
   on the proper divisors, and memoises the results.
   It works for any seq_t and idx_t provided by the build
   environment. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---------- helpers for factorisation and divisor generation ---------- */
static void factor_uint64(uint64_t n,
                          uint64_t primes[32],
                          int exps[32],
                          int *cnt)
{
    int c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (n > 1 && c < 32) {
        primes[c] = n;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* Generate all divisors of n from its prime factorisation.
   divs must have capacity at least 256.  Returns the number of
   divisors found.  The divisors are not sorted. */
static int gen_divisors(const uint64_t primes[32],
                        const int exps[32],
                        int cnt,
                        uint64_t divs[256])
{
    divs[0] = 1;
    int dcnt = 1;
    for (int i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        int cur = dcnt;
        uint64_t p_pow = 1;
        for (int k = 1; k <= e; ++k) {
            p_pow *= p;
            for (int j = 0; j < cur; ++j) {
                divs[dcnt++] = divs[j] * p_pow;
            }
        }
    }
    return dcnt;
}

/* ---------- main pseudoperfect test ---------- */
static bool is_pseudoperfect(uint64_t n)
{
    if (n <= 1) return false; /* 1 has no proper divisors */

    /* factorise n */
    uint64_t primes[32];
    int exps[32];
    int pf_cnt;
    factor_uint64(n, primes, exps, &pf_cnt);

    /* generate all divisors (including 1 and n) */
    uint64_t divs[256];
    int dcnt = gen_divisors(primes, exps, pf_cnt, divs);

    /* word count for bitset of size n+1 */
    size_t word_count = (n >> 6) + 1;
    uint64_t *bits = (uint64_t *)calloc(word_count, sizeof(uint64_t));
    if (!bits) return false; /* allocation failure -> conservatively no */
    bits[0] = 1ULL; /* sum 0 is reachable */

    uint64_t target_bits = 1ULL << (n & 63);
    size_t target_word = n >> 6;

    /* test each proper divisor (excluding n itself) */
    for (int i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if (d == n) continue; /* proper divisor only */

        size_t shift_words = d >> 6;
        unsigned shift_bits = d & 63;

        if (shift_words >= word_count) continue; /* shift beyond range */

        /* shift existing bits left by d and OR into bits */
        size_t i_word;
        for (i_word = word_count; i_word-- > shift_words; ) {
            size_t src_idx = i_word - shift_words;
            uint64_t part = bits[src_idx] << shift_bits;
            if (shift_bits != 0 && src_idx > 0) {
                part |= bits[src_idx - 1] >> (64 - shift_bits);
            }
            bits[i_word] |= part;
        }

        /* early exit if target reached */
        if (bits[target_word] & target_bits) {
            free(bits);
            return true;
        }
    }
    free(bits);
    return false;
}

/* ---------- memoised sequence builder ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t max_checked = 0;

static void cache_expand(void)
{
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, return 0 */

    /* provide cached results if available */
    if (cache_len >= (size_t)n)
        return cache[(size_t)(n - 1)];

    /* compute up to the required index */
    uint64_t candidate = max_checked + 1;
    while (cache_len < (size_t)n) {
        if (!is_pseudoperfect(candidate)) {
            cache_expand();
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    max_checked = candidate - 1;
    return cache[(size_t)(n - 1)];
}
