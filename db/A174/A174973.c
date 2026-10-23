/*
 * A174973  Numbers whose divisors increase by a factor of at most 2.
 * https://oeis.org/A174973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 7597, u32 n <= 9708, u64 n <= 9708,
 *            u128 n <= 9708, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174973 – numbers whose divisors increase by a factor ≤ 2.
   Computed by testing the prime‑factorisation condition:
   Let n = ∏ p_i^{e_i} with p_1 < p_2 < … .
   Then n belongs to the sequence iff for every i
        p_i ≤ 2 * ∏_{j<i} p_j^{e_j}.
   The function is memoised so that any order of calls works. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *spf = NULL;            /* smallest prime factor */
static size_t   spf_limit = 0;          /* current maximum index */

static uint32_t *prime_tbl = NULL;      /* list of primes */
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;

/* grow the spf array to contain indices up to at least limit (inclusive) */
static void grow_spf(size_t limit)
{
    if (limit <= spf_limit) return;

    free(spf);
    free(prime_tbl);

    spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!spf) abort();

    prime_cap = limit / 10 + 10;
    prime_tbl = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_tbl) abort();

    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) { /* i is prime */
            spf[(size_t)i] = (uint32_t)i;
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_tbl,
                                                     prime_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_tbl = tmp;
            }
            prime_tbl[prime_cnt++] = (uint32_t)i;
        }
        uint32_t spfi = spf[(size_t)i];
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_tbl[j];
            uint64_t v = (uint64_t)p * i;
            if (v > limit) break;
            spf[(size_t)v] = p;
            if (p == spfi) break;
        }
    }
    spf_limit = limit;
}

/* ensure the spf array covers at least value (inclusive) */
static void ensure_spf(uint64_t value)
{
    if (value <= spf_limit) return;
    size_t new_limit = spf_limit ? spf_limit * 2 : 64;
    while (new_limit < value) new_limit *= 2;
    grow_spf(new_limit);
}

/* check whether a number m satisfies the 2‑densely divisible condition */
static bool is_in_sequence(uint64_t m)
{
    if (m <= 1) return true;          /* 1 is the only odd term */

    ensure_spf(m);
    uint64_t t = m;
    seq_t product = 1;                /* ∏ previous prime powers */

    while (t > 1) {
        uint32_t p = spf[(size_t)t];
        uint32_t exp = 0;
        while (t % p == 0) {
            t /= p;
            ++exp;
        }
        seq_t pk = 1;
        for (uint32_t i = 0; i < exp; ++i) pk *= (seq_t)p;

        /* condition uses the prime value, not its power */
        if ((seq_t)p > 2 * product) return false;

        product *= pk;
    }
    return true;
}

/* cache of computed sequence values */
static seq_t *seq_cache   = NULL;
static size_t seq_cap     = 0;
static size_t seq_len     = 0;

static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;

    seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!newp) abort();

    for (size_t i = 0; i < seq_len; ++i) newp[i] = seq_cache[i];

    free(seq_cache);
    seq_cache = newp;
    seq_cap   = new_cap;
}

/* required external function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range – safety */
    size_t target = (size_t)n;

    if (seq_len < target) {
        uint64_t candidate = seq_len ? (uint64_t)seq_cache[seq_len - 1] + 1 : 1;

        while (seq_len < target) {
            if (candidate > spf_limit) ensure_spf(candidate);
            if (is_in_sequence(candidate)) {
                ensure_seq_cap(seq_len + 1);
                seq_cache[seq_len++] = (seq_t)candidate;
            }
            ++candidate;
        }
    }
    return seq_cache[target - 1];
}
