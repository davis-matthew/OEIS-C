/*
 * A094695  Numbers having in binary representation fewer ones than in their
 *          squares.
 * https://oeis.org/A094695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer such that popcount(n) < popcount(n²),
 *            1‑indexed
 * Range:     u8 n <= 198, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcnt_128, popcnt_generic, popcnt_generic128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence incrementally and caches results. */
/* It works for any unsigned integer type seq_t (including uint64_t and
   unsigned __int128) and idx_t as an unsigned 64‑bit type. */

#include <stdlib.h>
#include "../utils/popcnt_128.h"
#include "../utils/popcnt_generic.h"
#include "../utils/popcnt_generic128.h"

static seq_t *cache   = NULL;
static size_t  cap    = 0;
static size_t  cnt    = 0;
static idx_t   last_checked = 0;

/* Grow cache to at least min_cap entries. */
static void grow_cache(size_t min_cap) {
    if (cap >= min_cap) return;
    size_t new_cap = cap ? cap * 2 : 64;
    while (new_cap < min_cap) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* Compute popcount for seq_t values.  */
static unsigned int popcnt_seq(seq_t x) {
    return popcnt_generic((unsigned long long)x);
}

/* Main entry.  */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n==0, but guard against underflow */

    /* Extend cache until it contains at least n terms. */
    while (cnt < n) {
        idx_t cand = last_checked + 1;
        seq_t val  = (seq_t)cand;
        unsigned int pc_val = popcnt_seq(val);

        unsigned __int128 sq = (unsigned __int128)val * (unsigned __int128)val;
        unsigned int pc_sq = popcnt_128(sq);

        last_checked = cand;

        if (pc_val < pc_sq) {
            grow_cache(cnt + 1);
            cache[cnt++] = val;
        }
    }

    return cache[n - 1];
}
