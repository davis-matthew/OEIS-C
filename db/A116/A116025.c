/*
 * A116025  sigma(n) plus n gives a semiprime (A001358).
 * https://oeis.org/A116025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 857 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n such that sigma(n)+n is a semiprime (product of two primes, possibly equal). */
/* This implementation builds the sequence lazily and caches the results. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Cached sequence values: cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t  cache_len = 0;     /* number of values currently stored */

/* The next integer to test for the property.  The value is always > 0. */
static idx_t next_candidate = 1;

/* Return true iff m is a semiprime. */
static bool is_semiprime(seq_t m)
{
    /* 4 = 2*2 is the smallest semiprime. */
    if (m < 4) return false;

    if (is_prime_seq(m)) return false;   /* primes are not semiprimes */

    seq_t sqrt_m = floor_sqrt(m);
    if (sqrt_m < 2) return false;        /* guard against tiny values */

    /* Check divisors up to sqrt(m).  All arithmetic is in seq_t. */
    idx_t limit = (idx_t)sqrt_m;
    for (idx_t p = 2; p <= limit; ++p) {
        if (m % (seq_t)p == 0) {
            seq_t rem = m / (seq_t)p;
            return is_prime_seq(rem);
        }
    }
    return false;  /* should not reach here – m would be prime */
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Compute sigma(next_candidate) + next_candidate */
        idx_t cand = next_candidate;
        seq_t sum = sigma_of(cand) + (seq_t)cand;

        if (is_semiprime(sum)) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len] = cand;
            ++cache_len;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
