/*
 * A208530  Numbers n such that both n*Pi and n*e are within 1/sqrt(n) of
 *          integers.
 * https://oeis.org/A208530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 46, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: find the n‑th integer such that
   both |n*π−nearest integer| and |n*e−nearest integer| are < 1/√n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef __cplusplus
/* C++ supplies its own new/delete; in C we use malloc/free. */
#endif

/* Scaling factor for the rational approximations of π and e. */
static const unsigned long long SCALE     = 1000000000000000ULL;
static const unsigned long long PI_SCALE  = 3141592653589793ULL; /* floor(π*SCALE) */
static const unsigned long long E_SCALE   = 2718281828459045ULL; /* floor(e*SCALE) */

/* State used to build the sequence lazily. */
static idx_t         next_candidate = 1;   /* next integer to test */
static idx_t         found_cnt      = 0;   /* how many terms have been found */
static seq_t        *cache          = NULL;/* array of known terms */
static size_t        cache_cap      = 0;   /* capacity of the array */

/* --------------------------------------------------------------------- */
/* Helper: integer‑only test for a single n. */
static bool is_good(idx_t n)
{
    /* Distance of n*π to the nearest integer, scaled by SCALE. */
    unsigned __int128 raw_pi = (unsigned __int128)n * PI_SCALE;
    unsigned long long r_pi  = (unsigned long long)(raw_pi % SCALE);
    unsigned long long diff_pi = r_pi < (SCALE >> 1) ? r_pi : SCALE - r_pi;
    unsigned __int128 check_pi = (unsigned __int128)diff_pi * diff_pi * n;

    /* Distance of n*e to the nearest integer, scaled by SCALE. */
    unsigned __int128 raw_e  = (unsigned __int128)n * E_SCALE;
    unsigned long long r_e   = (unsigned long long)(raw_e % SCALE);
    unsigned long long diff_e = r_e < (SCALE >> 1) ? r_e : SCALE - r_e;
    unsigned __int128 check_e = (unsigned __int128)diff_e * diff_e * n;

    /* Constant SCALE^2. */
    static const unsigned __int128 SCALE_SQR = (unsigned __int128)SCALE * SCALE;

    return (check_pi < SCALE_SQR) && (check_e < SCALE_SQR);
}

/* --------------------------------------------------------------------- */
/* Grow the cache array to at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        delete [] cache;
    }
#else
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
#endif
    cache       = new_cache;
    cache_cap   = new_cap;
}

/* --------------------------------------------------------------------- */
/* Make the cache contain at least `needed` terms of the sequence. */
static void ensure_cache(idx_t needed)
{
    while (found_cnt < needed) {
        if (is_good(next_candidate)) {
            grow_cache(found_cnt + 1);
            cache[found_cnt++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* --------------------------------------------------------------------- */
/* Return the n-th term of A208530.  The first term (a(1)) is 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    ensure_cache(n);
    return cache[n - 1];
}
