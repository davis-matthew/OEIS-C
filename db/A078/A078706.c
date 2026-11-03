/*
 * A078706  a(n) = smallest integer >=a(n-1) such that sum of first n terms is
 *          prime.
 * https://oeis.org/A078706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 173, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9538 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A078706 – a(1)=2; for n>1 a(n) is the smallest integer 
   ≥a(n‑1) such that the sum of the first n terms is prime. */

#include <stdbool.h>
#include <stddef.h>              /* size_t */
#include <stdlib.h>              /* NULL */
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Static cache for the sequence values and their cumulative sums. */
static seq_t *a_cache   = NULL; /* a_cache[n] holds a(n)   (1‑based) */
static seq_t *sum_cache = NULL; /* sum_cache[n] holds a(1)+…+a(n) */
static idx_t  cached_n  = 0;     /* largest n that has been computed */
static size_t cache_cap = 0;     /* current capacity of the arrays */

/* Ensure that the cache can hold indices up to idx (inclusive). */
static void ensure_capacity(idx_t idx)
{
    size_t new_cap = (size_t)idx + 1;          /* +1 because indices start at 1 */
    if (new_cap <= cache_cap)
        return;

    seq_t *new_a   = seq_realloc(a_cache, cache_cap, new_cap);
    seq_t *new_sum = seq_realloc(sum_cache, cache_cap, new_cap);

    a_cache   = new_a;
    sum_cache = new_sum;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n=0, return 0 for safety */

    ensure_capacity(n);

    /* Compute all terms up to n that are not yet known. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        if (i == 1) {                /* first term */
            a_cache[1]   = 2;
            sum_cache[1] = 2;
        } else {
            seq_t candidate  = a_cache[i - 1];
            seq_t sum_so_far = sum_cache[i - 1];
            while (true) {
                seq_t new_sum = sum_so_far + candidate;
                if (isPrimeGeneric(new_sum)) {
                    a_cache[i]   = candidate;
                    sum_cache[i] = new_sum;
                    break;
                }
                ++candidate;
            }
        }
    }
    cached_n = n;
    return a_cache[n];
}
