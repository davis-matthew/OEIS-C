/*
 * A106794  Nonprimes of the form r(r(r(r(n)+1)+1)+1)+1, where
 *          A141468(n)=r(n)=n-th nonprime.
 * https://oeis.org/A106794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th nonprime among the values produced by * x = k;
 *            repeat 4 times: x = r(x) + 1, * where r(m) is the m‑th nonprime
 *            (A141468)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

/* Global static data for caching */
static seq_t *nonprimes = NULL;       /* table of n-th nonprimes */
static idx_t  nonprimes_size = 0;
static idx_t  nonprimes_cap  = 0;
static uint64_t next_checked = 0;     /* next integer to test for primality */

/* Global data for the a(n) cache */
static seq_t *a_cache = NULL;         /* array of nonprime outputs */
static idx_t  a_cache_size = 0;
static idx_t  a_cache_cap  = 0;
static idx_t  next_k = 1;             /* next k to evaluate (1‑based) */

/* Ensure nonprimes[0 .. needed-1] are filled (needed >= 1). */
static void ensure_nonprimes(idx_t needed)
{
    if (nonprimes_size >= needed) return;

    if (!nonprimes) {
        nonprimes_cap = 16;
        nonprimes = (seq_t *)malloc(nonprimes_cap * sizeof(seq_t));
        nonprimes[0] = (seq_t)0;          /* r(1) = 0 */
        nonprimes[1] = (seq_t)1;          /* r(2) = 1 */
        nonprimes_size = 2;
        next_checked = 2;
    }

    while (nonprimes_size < needed) {
        if (!is_prime_uint64(next_checked)) {
            if (nonprimes_size >= nonprimes_cap) {
                nonprimes_cap *= 2;
                nonprimes = (seq_t *)realloc(nonprimes, nonprimes_cap * sizeof(seq_t));
            }
            nonprimes[nonprimes_size++] = (seq_t)next_checked;
        }
        ++next_checked;
    }
}

/* Return the k‑th nonprime, k >= 1. */
static inline seq_t r_seq(idx_t k)
{
    ensure_nonprimes(k);
    return nonprimes[k - 1];
}

/* Ensure capacity of a_cache array. */
static void ensure_a_cache_cap(idx_t min_cap)
{
    if (a_cache_cap >= min_cap) return;
    idx_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
    while (new_cap < min_cap) new_cap *= 2;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but safe return */

    /* Generate more terms until we have at least n outputs. */
    while (a_cache_size < n) {
        /* Compute f(k) = apply r and +1 four times starting from k. */
        seq_t x = (seq_t)next_k;   /* x is a seq_t but used as idx_t */
        for (int i = 0; i < 4; ++i) {
            /* Need r(x) where x is an index; cast to idx_t safely. */
            idx_t idx = (idx_t)x;
            seq_t rval = r_seq(idx);
            x = rval + 1;
        }
        /* Check if resulting x is nonprime. */
        if ((uint64_t)x > UINT64_MAX) { /* very unlikely for required n */
            /* Treat as nonprime to be safe. */
            ensure_a_cache_cap(a_cache_size + 1);
            a_cache[a_cache_size++] = x;
        } else {
            if (!is_prime_uint64((uint64_t)x)) {
                ensure_a_cache_cap(a_cache_size + 1);
                a_cache[a_cache_size++] = x;
            }
        }
        ++next_k;
    }

    return a_cache[n - 1];
}
