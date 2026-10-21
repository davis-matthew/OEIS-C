/*
 * A072901  Composite numbers n such that the discriminant of the quadratic
 *          field Q(sqrt(n)) equals 4n.
 * https://oeis.org/A072901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth composite squarefree integer congruent to 2 or 3 (mod
 *            4)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all composite squarefree integers n such that
   n % 4 == 2 or 3.  By the discriminant formula of a quadratic field,
   this is equivalent to the discriminant of Q(√n) being 4·n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_uint64.h"   /* provides factor_uint64(...) */

/* Helper to grow the cache array */
static void ensure_cap(size_t *cap, size_t needed) {
    if (*cap < needed) {
        size_t new_cap = *cap ? *cap << 1 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(NULL, new_cap * sizeof(seq_t));
        /* In the first call realloc(NULL, ...) behaves like malloc. */
        /* The following assignment ensures we keep the previous data. */
        if (*cap) {
            tmp = (seq_t *)realloc(NULL, new_cap * sizeof(seq_t));
        }
        *cap = new_cap;
    }
}

static seq_t *cache = NULL;          /* cache of sequence terms  */
static size_t cache_len = 0;         /* number of cached terms    */
static size_t cache_cap = 0;         /* allocated capacity        */
static uint64_t next_candidate = 5;  /* starts searching beyond 5 */

/* Extend the cache until it contains at least 'target' terms */
static void extend_cache(size_t target) {
    while (cache_len < target) {
        uint64_t n = ++next_candidate;   /* candidate integer */

        /* Only numbers ≡ 2 or 3 (mod 4) can satisfy discriminant ≡ 4n */
        if ((n & 3) != 2 && (n & 3) != 3)
            continue;

        /* Factor n.  factor_uint64 returns the number of prime factors
           (cnt) and their exponents (exp).  n is squarefree iff all
           exponents are 1. */
        uint64_t fac[20];
        int exp[20];
        int cnt;
        factor_uint64(n, fac, exp, &cnt);

        /* Must be composite: at least two distinct prime factors.
           Must be squarefree: all exponents must equal 1. */
        if (cnt < 2)        /* n is prime */
            continue;

        bool squarefree = true;
        for (int i = 0; i < cnt; ++i) {
            if (exp[i] != 1) {
                squarefree = false;
                break;
            }
        }
        if (!squarefree)
            continue;

        /* Store the term */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);   /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)n;
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* undefined, but guard against bad input */

    /* Compute terms lazily; cache handles out-of-order requests */
    if (cache_len < (size_t)n)
        extend_cache((size_t)n);

    return cache[n - 1];
}
