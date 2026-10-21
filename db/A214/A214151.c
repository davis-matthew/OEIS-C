/*
 * A214151  Numbers k from the set == 5 (mod 6) with the property that
 *          3^((3*k-1)/2) == 3 (mod k) and 2^((k-1)/2) == (k-1) (mod k).
 * https://oeis.org/A214151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 822, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A214151: numbers k ≡ 5 (mod 6) such that
   3^((3k-1)/2) ≡ 3 (mod k)  and
   2^((k-1)/2) ≡ k-1 (mod k).  Search candidates sequentially,
   storing found values for future calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* Cache of discovered terms. */
static seq_t *cache     = NULL;
static idx_t cache_len  = 0;
static idx_t cache_cap  = 0;
static seq_t next_candidate = 11;   /* first candidate (≡5 mod 6, >5) */

/* Grow the cache array to hold at least "need" elements. */
static void
ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache     = tmp;
    cache_cap = new_cap;
}

/* Test whether k satisfies the two congruences. */
static bool
good_candidate(seq_t k)
{
    /* (k-1)/2 fits into a 64‑bit unsigned.  It is safe for the
       expected size of k in the production build. */
    uint64_t e2 = (uint64_t)((k * 3ULL - 1) / 2);   /* (3k-1)/2 */
    uint64_t e1 = (uint64_t)((k - 1) / 2);          /* (k-1)/2 */

    /* 2^((k-1)/2) ≡ k-1 (mod k)  ⇔  ≡ -1 (mod k) */
    if (mod_pow((seq_t)2, e1, k) != (k - 1))
        return false;

    /* 3^((3k-1)/2) ≡ 3 (mod k) */
    return mod_pow((seq_t)3, e2, k) == 3U;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Scan candidates, stepping by 6 (maintain 5 mod 6). */
        while (true) {
            seq_t k = next_candidate;
            if (good_candidate(k)) {
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = k;
                next_candidate = k + 6;
                break;
            }
            next_candidate += 6;
        }
    }
    return cache[n - 1];
}
