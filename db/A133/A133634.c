/*
 * A133634  Nonprime numbers k such that binomial(k+p,k) mod k = 1, where p=4.
 * https://oeis.org/A133634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the nth nonprime integer k such that
   C(k+4,4) mod k == 1. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;   /* array of found terms              */
static idx_t  len   = 0;       /* number of terms stored             */
static idx_t  cap   = 0;       /* capacity of the array             */
static idx_t  next_test = 2;   /* next integer to test for criteria */

static void grow_cache(idx_t needed)
{
    if (needed <= len) return;

    /* enlarge the array if necessary */
    if (cap < needed) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();            /* out of memory */
        cache = p;
        cap   = (idx_t)new_cap;
    }

    /* test consecutive integers until we have enough terms */
    idx_t k;
    for (k = next_test; len < needed; ++k) {
        if (is_prime(k)) continue;        /* skip prime k */

        /* compute binomial(k+4,4) = (k+1)(k+2)(k+3)(k+4)/24 */
        unsigned __int128 prod = (unsigned __int128)(k + 1)
                               * (k + 2) * (k + 3) * (k + 4);
        prod /= 24;                         /* division is exact */

        if ((unsigned __int128)(prod % k) == 1) {  /* condition ok */
            cache[len++] = (seq_t)k;
        }
    }
    next_test = k;   /* remember where we stopped */
}

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n<1, safe guard */
    grow_cache(n);
    return cache[n - 1];
}
