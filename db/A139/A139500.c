/*
 * A139500  Primes of the form x^2 + 20x*y + y^2 for x and y nonnegative.
 * https://oeis.org/A139500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139500: primes of the form x^2 + 20xy + y^2 with x,y ≥ 0 */
/* The function returns the nth prime in ascending order (1‑based index). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

/* ---------- cache for discovered primes ---------- */
static seq_t *prime_cache   = NULL;      /* array of discovered primes   */
static size_t prime_cap     = 0;          /* allocated capacity          */
static size_t prime_len     = 0;          /* number of primes stored     */
static idx_t  cache_limit   = 0;          /* last numeric limit processed */
static seq_t  processed_limit_seq = 0;    /* same limit but in seq_t form */

/* Ensure the prime cache has at least min_cap elements. */
static void ensure_prime_capacity(size_t min_cap) {
    if (prime_cap >= min_cap)
        return;
    size_t new_cap = (prime_cap < 8) ? 8 : prime_cap * 2;
    while (new_cap < min_cap)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    prime_cache = tmp;
    prime_cap   = new_cap;
}

/* Append a prime value to the cache. */
static void append_prime(seq_t val) {
    ensure_prime_capacity(prime_len + 1);
    prime_cache[prime_len++] = val;
}

/* Comparator for qsort on seq_t values. */
static int cmp_seq(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x < y) ? -1 : (x > y);
}

/* Expand the cache to include all prime values ≤ new_limit (numeric). */
static void expand_cache(idx_t new_limit) {
    seq_t limit_seq = (seq_t)new_limit;

    /* Enumerate pairs (x, y) producing values ≤ limit_seq. */
    for (idx_t y = 0; ; ++y) {
        seq_t y2 = (seq_t)y * (seq_t)y;
        if (y2 > limit_seq)
            break;                           /* y is too large */
        for (idx_t x = 0; ; ++x) {
            seq_t val = (seq_t)x * (seq_t)x +
                        (seq_t)20 * (seq_t)x * (seq_t)y + y2;
            if (val > limit_seq)
                break;                       /* further x give larger values */
            if (val <= processed_limit_seq)
                continue;                    /* already processed */
            if (isPrimeGeneric(val))
                append_prime(val);
        }
    }

    /* Sort the cache if needed. */
    if (prime_len > 1)
        qsort(prime_cache, prime_len, sizeof(seq_t), cmp_seq);

    /* Deduplicate successive equal values. */
    if (prime_len > 1) {
        size_t j = 0;
        for (size_t i = 1; i < prime_len; ++i) {
            if (prime_cache[i] != prime_cache[j]) {
                ++j;
                prime_cache[j] = prime_cache[i];
            }
        }
        prime_len = j + 1;
    }

    cache_limit          = new_limit;
    processed_limit_seq  = limit_seq;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* undefined; return 0 by convention */

    /* Ensure enough primes are in the cache. */
    idx_t lim = (cache_limit == 0) ? 1000 : cache_limit * 2;
    while (prime_len < n) {
        /* guard against overflow of the numeric limit. */
        if (lim < cache_limit && cache_limit != 0)
            break;                  /* overflow detected */
        expand_cache(lim);
        if (lim >= UINT64_MAX / 2)
            lim = UINT64_MAX;
        else
            lim *= 2;
    }

    /* Return the nth prime (1‑based). */
    return prime_cache[n - 1];
}
