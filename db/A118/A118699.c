/*
 * A118699  Smaller of two consecutive semiprimes with the same digital root.
 * https://oeis.org/A118699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 370, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118699 – smaller of two consecutive semiprimes with the same digital root.
   The function generates terms by scanning integers, testing for semiprimality with
   a pre‑computed smallest prime factor table, computing the digital root in the
   simple integer domain, and storing every pair's smaller member. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"

/* Upper bound for the sieve.  This comfortably covers the first 10 000 terms. */
static const idx_t MAX_LIMIT = 20000000;

/* --- Static state -------------------------------------------------------- */
static bool initialized = false;
static idx_t *spf = NULL;          /* spf[i] = smallest prime factor of i */
static seq_t *cache = NULL;        /* cached sequence terms */
static size_t cache_cap = 0;
static idx_t cache_len = 0;        /* number of terms actually cached */

static idx_t current = 4;          /* next integer to test */
static idx_t prev_semiprime = 0;   /* previous semiprime */
static int    prev_dr = -1;        /* its digital root */
static bool   had_prev = false;    /* whether we have a previous semiprime */

/* --- Helper functions ---------------------------------------------------- */
static inline int digital_root(idx_t x)
{
    /* compute the sum of decimal digits; idx_t is small enough for this work */
    unsigned int sum = 0;
    while (x > 0) {
        sum += (unsigned int)(x % 10);
        x /= 10;
    }
    int dr = sum % 9;
    return dr ? dr : 9;
}

static inline bool is_semiprime(idx_t n)
{
    if (n < 4) return false;
    if (n <= MAX_LIMIT) {
        idx_t p = spf[n];
        if (p == n) return false;                /* n itself is prime */
        idx_t q = n / p;
        return (q <= MAX_LIMIT) ? (spf[q] == q) : false;
    }
    /* Numbers beyond the sieve are not expected for the required range. */
    return false;
}

static void init(void)
{
    if (initialized) return;
    build_spf(MAX_LIMIT, &spf);
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();
    initialized = true;
}

static void push_cache(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init();

    /* Return cached value if already known */
    if (n > 0 && n <= cache_len) return cache[n - 1];

    /* Generate terms until we have the required number */
    while (cache_len < n && current <= MAX_LIMIT) {
        if (is_semiprime(current)) {
            int dr = digital_root(current);
            if (had_prev && dr == prev_dr)
                push_cache((seq_t)prev_semiprime);
            prev_semiprime = current;
            prev_dr = dr;
            had_prev = true;
        }
        ++current;
    }

    /* If the requested term is beyond our pre‑computed range, return 0. */
    return (cache_len >= n) ? cache[n - 1] : (seq_t)0;
}
