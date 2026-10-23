/*
 * A025004  a(1) = 3; a(n+1) = a(n)-th nonprime, where nonprimes begin at 1.
 * https://oeis.org/A025004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   odd_bit_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 3; a(n+1) = the a(n)-th nonprime,
 * where nonprimes are counted starting from 1 (1,4,6,8,9,10,12,...). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../utils/odd_bit_index.h"

static const idx_t SIEVE_LIMIT = 500000000ULL;   /* 5·10^8 */

/* --------- Bit‑set for odd numbers 3,5,7,9,... --------- */
static uint8_t *sieve_bits = NULL;   /* each bit = 1 → prime, 0 → composite */

static inline bool bit_get(idx_t n) {          /* n odd >=3 */
    size_t idx = odd_bit_index(n);
    return (sieve_bits[idx >> 3] >> (idx & 7)) & 1;
}
static inline void bit_clear(idx_t n) {         /* set to composite */
    size_t idx = odd_bit_index(n);
    sieve_bits[idx >> 3] &= ~(1u << (idx & 7));
}

/* Build a sieve of Eratosthenes up to SIEVE_LIMIT.
 * Only odd numbers are stored.  */
static void init_sieve(void) {
    size_t odd_count = (SIEVE_LIMIT + 1) >> 1;        /* count of odd numbers >=3 */
    size_t byte_count = (odd_count + 7) >> 3;         /* in bytes */
    sieve_bits = (uint8_t *)malloc(byte_count);
    if (!sieve_bits) exit(1);                         /* out of memory */
    memset(sieve_bits, 0xFF, byte_count);            /* all odd numbers prime */

    uint64_t limit = SIEVE_LIMIT;
    uint64_t sqrt_limit = (uint64_t)floor(sqrt((double)limit));
    for (uint64_t p = 3; p <= sqrt_limit; p += 2) {
        if (bit_get(p)) {
            uint64_t step = p << 1;                  /* 2p */
            for (uint64_t m = p * p; m <= limit; m += step)
                bit_clear(m);
        }
    }
}

/* Test whether n is prime using the bitset; works for n<=SIEVE_LIMIT. */
static inline bool is_prime_candidate(idx_t n) {
    if (n == 2) return true;
    if ((n & 1u) == 0) return false; /* even number >2 */
    if (n < 3) return false;          /* 0,1 are not prime */
    if (n > SIEVE_LIMIT) return false;/* beyond sieve – should not happen */
    return bit_get(n);
}

/* Global state for incremental search.  */
static idx_t max_checked   = 0;   /* largest number examined */
static idx_t nonprime_cnt  = 0;   /* number of nonprimes found up to max_checked */

/* Find the k‑th nonprime (k≥1).  On return, max_checked = value found. */
static seq_t find_kth_nonprime(idx_t k) {
    while (true) {
        ++max_checked;
        bool composite;
        if (max_checked == 1) {
            composite = true;           /* 1 is nonprime */
        } else {
            composite = !is_prime_candidate(max_checked);
        }
        if (composite) {
            ++nonprime_cnt;
            if (nonprime_cnt == k)
                return max_checked;
        }
    }
}

/* --------- Memoised sequence cache  --------- */
static seq_t *seq_cache      = NULL;   /* array of computed terms */
static idx_t  cache_cap      = 0;      /* capacity of seq_cache */
static idx_t  terms_computed = 0;      /* number of terms stored */

static void ensure_cache_capacity(idx_t n) {
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 1;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                  /* allocation failure */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* --------- Sequence entry point  --------- */
seq_t a(idx_t n) {
    /* Initialise sieve and cache on first call. */
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        init_sieve();
        ensure_cache_capacity(1);
        seq_cache[0] = (seq_t)3;        /* a(1) */
        terms_computed = 1;
    }

    if (n <= terms_computed)           /* cached result */
        return seq_cache[n - 1];

    ensure_cache_capacity(n);

    for (idx_t i = terms_computed + 1; i <= n; ++i) {
        seq_t prev = seq_cache[i - 2];          /* a(i-1) */
        idx_t target = (idx_t)prev;             /* index for next nonprime */
        seq_t val = find_kth_nonprime(target);
        seq_cache[i - 1] = val;
    }
    terms_computed = n;
    return seq_cache[n - 1];
}
