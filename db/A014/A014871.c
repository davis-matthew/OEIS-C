/*
 * A014871  Numbers k that divide s(k), where s(1)=1, s(j)=20*s(j-1)+j.
 * https://oeis.org/A014871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A014871:  Numbers k that divide s(k), where
    s(1) = 1,
    s(j) = 20*s(j-1) + j  (j ≥ 2)
    The function returns the n‑th such k (1‑based). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_idx.h"

/* Test whether k divides s(k).  Uses the equivalence: k | s(k)
   ⇔ s(k-1) ≡ 0 (mod k / gcd(k,20)). */
static inline int _divides_s(idx_t k)
{
    if (k == 1) return 1;                  /* s(1) = 1 is divisible by 1 */

    idx_t g = gcd_idx(k, (idx_t)20);
    idx_t m = k / g;                       /* modulus to test */

    if (m == 1) return 1;                  /* always satisfied */

    idx_t rem = 1 % m;                     /* s(1) modulo m */
    for (idx_t j = 2; j <= k-1; ++j) {
        /* rem = (20*rem + j) mod m */
        unsigned __int128 tmp = (unsigned __int128)rem * 20 + j;
        rem = (idx_t)(tmp % m);
    }
    return rem == 0;
}

/* --------------------------------------------------------------------- */
/* Helper: greatest common divisor for idx_t (uint64_t). */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Cache for previously found terms.  Terms are stored as idx_t.
   a[0] holds a(1).  next_candidate starts at 1. */
static idx_t *a_cache   = NULL;            /* array of found k's */
static size_t a_cache_sz  = 0;            /* number of stored terms   */
static size_t a_cache_cap = 0;            /* capacity of the array   */
static idx_t  next_candidate = 1;         /* next integer to test    */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    /* Grow the cache until we have n terms. */
    while (a_cache_sz < n) {
        /* Find the next k that satisfies the condition. */
        while (!_divides_s(next_candidate))
            ++next_candidate;

        /* Allocate more space if necessary. */
        if (a_cache_sz == a_cache_cap) {
            size_t new_cap = (a_cache_cap == 0) ? 64 : a_cache_cap * 2;
            idx_t *tmp = (idx_t *)realloc(a_cache, new_cap * sizeof(idx_t));
            if (!tmp) abort();              /* memory exhaustion */
            a_cache = tmp;
            a_cache_cap = new_cap;
        }

        a_cache[a_cache_sz++] = next_candidate;
        ++next_candidate;                  /* next candidate to test */
    }

    return (seq_t)a_cache[n - 1];          /* cast to seq_t for the caller */
}
