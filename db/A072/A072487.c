/*
 * A072487  a(1) = 1, a(n) = a(n-1) times largest nontrivial divisor if n is
 *          composite.
 * https://oeis.org/A072487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1.
   For n > 1:
       if n is prime:      a(n) = n * a(n-1)
       else (composite):   let p = smallest prime factor of n.
                           a(n) = (n/p) * a(n-1)   // largest proper divisor = n/p
   The function caches computed values so that calls in any order remain
   fast (overall O(N) in the largest n requested). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build. */
static seq_t *a_cache = NULL;             /* array of computed a(n) */
static size_t cache_cap = 0;              /* allocated capacity, including index 0 */
static idx_t max_computed = 0;            /* largest n for which a_cache[n] is valid */

/* smallest prime factor array; lpf[i] == i for prime i */
static uint64_t *lpf = NULL;              /* size cache_cap */
static size_t lpf_cap = 0;

/* ------------------------------------------------------------------ */
/* Utility: extend the lpf array up to new_limit (inclusive).         */
/* This recomputes lpf[2..new_limit] by trial division (fast for |n|≤10⁴). */
static void extend_lpf(idx_t new_limit)
{
    if (new_limit < 2) return;

    if (new_limit + 1 > lpf_cap) {
        if (lpf == NULL) {
            lpf = (uint64_t*)malloc(sizeof(uint64_t) * (new_limit + 1));
            lpf_cap = new_limit + 1;
        } else {
            lpf = (uint64_t*)realloc(lpf, sizeof(uint64_t) * (new_limit + 1));
            lpf_cap = new_limit + 1;
        }
        /* initialize: each number is its own smallest prime factor */
        for (idx_t i = 2; i <= new_limit; ++i) lpf[i] = (uint64_t)i;
    }

    /* Sieve for smallest prime factor */
    idx_t limit = new_limit;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (lpf[i] == i) {                 /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (lpf[j] == j) lpf[j] = i;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Ensure the a_cache array holds values up to n (inclusive).          */
static void ensure_cache(idx_t n)
{
    if (n <= max_computed) return;

    /* Extend lpf array */
    extend_lpf(n);

    /* Extend a_cache array */
    if (n + 1 > cache_cap) {
        a_cache = (seq_t*)realloc(a_cache, sizeof(seq_t) * (n + 1));
        cache_cap = n + 1;
    }

    /* Initialise for indices 0 and 1 if needed */
    if (max_computed < 1) {
        if (cache_cap > 1) a_cache[1] = (seq_t)1;
        max_computed = 1;
    }

    /* Compute new a values */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev = a_cache[i - 1];
        seq_t mult;
        if (lpf[i] == i) {                     /* prime */
            mult = (seq_t)i;
        } else {
            /* composite: largest proper divisor = i / smallest_prime_factor */
            uint64_t divisor = i / lpf[i];
            mult = (seq_t)divisor;
        }
        a_cache[i] = prev * mult;
    }

    max_computed = n;
}

/* ------------------------------------------------------------------ */
/* Public routine */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n<1, but return 0 */
    ensure_cache(n);
    return a_cache[n];
}
