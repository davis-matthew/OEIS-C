/*
 * A161533  The smallest of three consecutive primes p1 < p2 < p3, where
 *          p2-p1, p3-p2, and p3-p1 are all perfect squares.
 * https://oeis.org/A161533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A161533: the smallest of three consecutive primes p1<p2<p3
   where p2-p1, p3-p2 and p3-p1 are all perfect squares.
   This implementation searches successively for consecutive prime triples
   and stores the smallest primes in a static cache.  It works for any
   seq_t and idx_t supplied by the build, avoiding a realloc of
   non‑POD types by keeping the cache in uint64_t and only converting to
   seq_t on return.  The algorithm runs in O(N) time per term and
   is thread‑safe only for a single sequence generator. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_floor_isqrt_u64.h"

/* ----------------- dynamic array helpers ----------------- */
static inline void grow_uint64(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    *cap = new_cap;
}

/* ----------------- segmented sieve ----------------- */
#define SEG_SZ (1 << 20)          /* 1,048,576 numbers per segment */

static uint64_t *prime_list   = NULL; /* all primes found so far */
static size_t    primes_cap   = 0;
static size_t    primes_used  = 0;

static uint64_t *base_primes  = NULL; /* primes up to sqrt(high) */
static size_t    base_cap     = 0;
static size_t    base_used    = 0;

static uint64_t low_bound = 2;        /* start of next segment */

/* precompute small primes up to 10^6 (sufficient for square roots up
   to 10^12, more are added lazily if needed). */
static void init_base_primes(void)
{
    if (base_used) return;            /* already initialized */
    const uint32_t LIMIT = 1000000;
    uint8_t *comp = (uint8_t *)calloc(LIMIT + 1, 1);
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i)
        if (!comp[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                comp[j] = 1;
    grow_uint64(&base_primes, &base_cap, LIMIT / 10);
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!comp[i]) base_primes[base_used++] = i;
    free(comp);
}

static void generate_next_segment(void)
{
    uint64_t high = low_bound + SEG_SZ;
    uint8_t *seg = (uint8_t *)calloc(SEG_SZ, 1);

    for (size_t pi = 0; pi < base_used; ++pi) {
        uint32_t p = base_primes[pi];
        if ((uint64_t)p * p > high) break;
        uint64_t start = (low_bound + p - 1) / p * p;
        if (start < (uint64_t)p * p) start = (uint64_t)p * p;
        for (uint64_t j = start; j < high; j += p)
            seg[j - low_bound] = 1;
    }
    if (low_bound <= 1 && high > 1) seg[1 - low_bound] = 1; /* mark 1 */

    for (uint32_t i = 0; i < SEG_SZ; ++i) {
        uint64_t num = low_bound + i;
        if (num >= 2 && !seg[i]) {
            grow_uint64(&prime_list, &primes_cap, primes_used + 1);
            prime_list[primes_used++] = num;
        }
    }
    free(seg);
    low_bound = high;
}

/* Ensure that at least 'idx+3' primes are available (for a window
   starting at index 'idx'). */
static void ensure_primes_at(size_t idx)
{
    while (primes_used < idx + 3)
        generate_next_segment();
}

/* ----------------- cache of found terms ----------------- */
static uint64_t *term_cache  = NULL; /* store as uint64_t */
static size_t    terms_cap   = 0;
static size_t    terms_used  = 0;
static size_t    window_start = 0;   /* current start index of the window */

/* ----------------- square test ----------------- */
static inline bool is_square_u64(uint64_t x)
{
    uint64_t r = _floor_isqrt_u64(x);
    return r * r == x;
}

/* compute the sequence up to n terms */
static void compute_up_to(idx_t n)
{
    while (terms_used < n) {
        ensure_primes_at(window_start);
        uint64_t p1 = prime_list[window_start];
        uint64_t p2 = prime_list[window_start + 1];
        uint64_t p3 = prime_list[window_start + 2];
        uint64_t d1 = p2 - p1;
        uint64_t d2 = p3 - p2;
        if (is_square_u64(d1) && is_square_u64(d2) &&
            is_square_u64(d1 + d2)) {
            grow_uint64(&term_cache, &terms_cap, terms_used + 1);
            term_cache[terms_used++] = p1;
        }
        ++window_start;
    }
}

/* ----------------- public interface ----------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n=0 */

    if (!term_cache) {                     /* initialize cache */
        terms_cap = 16;
        term_cache = (uint64_t *)malloc(terms_cap * sizeof(uint64_t));
    }
    if (!prime_list) {                     /* initialize prime list */
        primes_cap = 1024;
        prime_list = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }

    init_base_primes();
    compute_up_to(n);
    return (seq_t)term_cache[n - 1];
}
