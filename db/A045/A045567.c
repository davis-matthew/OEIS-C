/*
 * A045567  Numbers whose factorial has '84' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 3305, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2_mod_100
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045567:
   Numbers whose factorial has the two non‑zero trailing digits 84.
   Computation: for each m we keep the product of all numbers
   i = 1..m after removing all factors 5 and 2 from i, and we
   keep the total counts of removed 2s (cnt2) and 5s (cnt5).
   The trimmed factorial equals prod * 2^(cnt2-cnt5) mod 100.
   A term is stored when that value is 84.  */
/* The implementation stores previously found terms in a static cache
   so that calls with arbitrary n are fast.  */

#include <stdlib.h>
#include "../utils/pow2_mod_100.h"

static seq_t *cache = NULL;          /* array of found terms */
static size_t cache_cap = 0;         /* allocated cache size */
static size_t cache_len = 0;         /* number of terms stored */

static idx_t curr_m = 0;             /* last processed m */
static seq_t prod_mod = 1;           /* product modulo 100, after removing 2s and 5s */
static idx_t cnt2 = 0;               /* total number of 2s removed */
static idx_t cnt5 = 0;               /* total number of 5s removed */

/* Grow cache to accommodate at least min_cap elements.  */
static void grow_cache(size_t min_cap) {
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a new found term to the cache.  */
static void cache_append(seq_t val) {
    grow_cache(cache_len + 1);
    cache[cache_len++] = val;
}

/* Main sequence function.  */
seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* offset is 1, lower values undefined */

    /* Generate terms until we have at least n of them.  */
    while (cache_len < (size_t)n) {
        ++curr_m;
        idx_t t = curr_m;

        /* Remove all factors of 5 from t, counting them.  */
        while (t % 5U == 0U) { t /= 5U; ++cnt5; }

        /* Remove all factors of 2 from t, counting them.  */
        while (t % 2U == 0U) { t /= 2U; ++cnt2; }

        /* Multiply the remaining odd factor into prod_mod (mod 100).  */
        prod_mod = (prod_mod * (seq_t)(t % 100U)) % 100;

        /* Compute the trimmed factorial modulo 100.  */
        idx_t extra = cnt2 - cnt5;          /* non‑negative by Legendre’s formula */
        seq_t trimmed = (prod_mod * pow2_mod_100(extra)) % 100;

        /* If last two non‑zero digits are 84, store the term.  */
        if (trimmed == (seq_t)84U) cache_append((seq_t)curr_m);
    }

    return cache[n - 1];
}
