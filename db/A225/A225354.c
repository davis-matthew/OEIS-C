/*
 * A225354  Numbers that can be written as sum of distinct squarefree
 *          divisors.
 * https://oeis.org/A225354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   shift_left
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that can be written as sum of distinct squarefree divisors.
   Computed by searching integers and testing representability using a subset‑sum
   bitset over the squarefree divisors of each integer. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/shift_left.h"

/* ---- global cache for the sequence ------------------------------------- */
static seq_t *seq_cache = NULL;   /* array of found terms                  */
static size_t seq_len   = 0;       /* number of terms stored                */
static size_t seq_cap   = 0;       /* allocated capacity                    */

/* ---- smallest prime factor table --------------------------------------- */
static uint32_t *spf      = NULL;  /* spf[i] = smallest prime factor of i  */
static size_t   spf_limit = 0;     /* current maximum value covered         */

static void init_spf(size_t limit)
{
    spf_limit = limit;
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i)
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = (uint32_t)i;
    }
}

static void ensure_spf(size_t limit)
{
    if (spf == NULL) {
        init_spf(limit);
        return;
    }
    if (limit <= spf_limit) return;

    size_t new_limit = spf_limit*2;
    if (new_limit < limit) new_limit = limit;
    spf = (uint32_t *)realloc(spf, (new_limit + 1) * sizeof(uint32_t));
    for (size_t i = spf_limit + 1; i <= new_limit; ++i) spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (spf[i] == i)
            for (size_t j = i * i; j <= new_limit; j += i)
                if (spf[j] == j) spf[j] = (uint32_t)i;
    }
    spf_limit = new_limit;
}

/* ---- gather distinct prime factors of n --------------------------------- */
static void distinct_primes(uint64_t n, uint32_t primes[], size_t *pcount)
{
    *pcount = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        primes[(*pcount)++] = p;
        while (n % p == 0) n /= p;
    }
}

/* ---- subset‑sum bitset utilities ---------------------------------------- */
static uint64_t *dp_bits  = NULL;
static uint64_t *tmp_bits = NULL;
static size_t    dp_len   = 0;

static void ensure_dp(size_t N)
{
    size_t words = (N >> 6) + 1;
    if (dp_len >= words) return;
    dp_bits  = (uint64_t *)realloc(dp_bits,  words * sizeof(uint64_t));
    tmp_bits = (uint64_t *)realloc(tmp_bits, words * sizeof(uint64_t));
    dp_len   = words;
    /* ensure memory cleared before first use */
    memset(dp_bits, 0, dp_len * sizeof(uint64_t));
    memset(tmp_bits, 0, dp_len * sizeof(uint64_t));
}

/* ---- test if n can be written as a sum of distinct squarefree divisors --- */
static int can_represent(uint64_t n)
{
    if (n == 1) return 1;    /* 1 = 1 */

    ensure_spf(n);

    /* distinct prime factors of n */
    uint32_t primes[32];
    size_t pcount;
    distinct_primes(n, primes, &pcount);

    /* all squarefree divisors: products of subsets of primes */
    uint64_t divs[1 << 16];   /* > 2^16 > 65536 divisors, more than enough */
    size_t divcnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < pcount; ++i) {
        uint64_t p = primes[i];
        size_t old = divcnt;
        for (size_t j = 0; j < old; ++j)
            divs[divcnt++] = divs[j] * p;
    }

    /* subset‑sum DP */
    ensure_dp(n);
    for (size_t i = 0; i < dp_len; ++i) dp_bits[i] = 0ULL;
    dp_bits[0] = 1ULL;            /* sum 0 achievable */

    uint32_t target_word = n >> 6;
    uint32_t target_bit  = n & 63;
    uint64_t target_mask = 1ULL << target_bit;

    for (size_t i = 0; i < divcnt; ++i) {
        uint32_t d = (uint32_t)divs[i];
        if (d == 0) continue;
        shift_left(dp_bits, tmp_bits, dp_len, d);
        for (size_t w = 0; w < dp_len; ++w) dp_bits[w] |= tmp_bits[w];
        if (dp_bits[target_word] & target_mask) return 1;
    }
    return 0;
}

/* ---- append a term to the cache ---------------------------------------- */
static void cache_append(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* ---- main sequence function --------------------------------------------- */
static uint64_t next_candidate = 1;   /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but avoid crash */

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        if (can_represent(next_candidate))
            cache_append((seq_t)next_candidate);
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
