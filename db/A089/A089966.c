/*
 * A089966  Numbers k such that omega(k) = gpf(k) mod lpf(k), where
 *          omega=A001221, gpf=A006530 and lpf=A020639.
 * https://oeis.org/A089966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer k such that ω(k) = gpf(k) (mod
 *            lpf(k)), where ω is the number of distinct prime factors, gpf
 *            the greatest prime factor, and lpf the smallest prime factor
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   largest_prime_factor, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility functions provided by the build environment. */
#include <stdlib.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/omega.h"

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
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

/* Test whether a number satisfies the defining condition. */
static inline bool satisfies(uint64_t k)
{
    /* k == 1 is considered to satisfy the predicate. */
    if (k == 1)
        return true;

    uint64_t l = lpf((idx_t)k);                 /* lpf(k) */
    uint64_t g = largest_prime_factor(k);                        /* gpf(k) */
    int      o = omega(k);      /* ω(k) */

    /* (g mod l) is computed with uint64_t and then compared to ω(k). */
    uint64_t mod = g % l;
    return (uint64_t)o == mod;
}

/* Append a value to a dynamic array, reallocating if necessary. */
static inline void push(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = seq_realloc(*arr, *len, new_cap);
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    ++(*len);
}

/* Public sequence function.  It maintains a static cache so that
   repeated calls are fast, regardless of the order of the indices. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* array of values found so far */
    static size_t len = 0;      /* number of entries in seq       */
    static size_t cap = 0;      /* allocated capacity of seq       */
    static uint64_t cur = 1;    /* next integer to examine (1‑based) */

    while (len < (size_t)n) {
        if (satisfies(cur))
            push(&seq, &len, &cap, (seq_t)cur);
        ++cur;
    }
    return seq[(size_t)n - 1];
}
