/*
 * A137671  a(n+1) is the number of preceding terms having in binary
 *          representation the same number of ones as a(n), starting with a(1)
 *          = 1.
 * https://oeis.org/A137671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1. For n ≥ 1, a(n+1) = #{ i ≤ n | popcount(a(i)) =
 *            popcount(a(n)) } (recurrence)
 * Range:     u8 n <= 961, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* popcount for 64‑bit values */
static inline unsigned popcnt64(uint64_t x) {
    return __builtin_popcountll(x);
}

/* static state */
static seq_t *cache = NULL;       /* computed terms a(1)..a(len) */
static size_t len = 0;            /* number of computed terms */
static size_t cap = 0;            /* allocated capacity of cache    */
static unsigned freq[65] = {0};   /* frequency of popcounts seen so far (popcnt ≤ 64) */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* not used, but defined for safety */

    /* initialise with a(1) = 1 */
    if (len == 0) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        len = 1;
        freq[popcnt64(1)] = 1;
    }

    /* fill cache up to needed index */
    while (len < (size_t)n) {
        /* compute a(len+1) using popcount of a(len) */
        unsigned pc_current = popcnt64((uint64_t)cache[len - 1]);   /* popcount(a(len)) */
        seq_t next = (seq_t)freq[pc_current];                      /* a(len+1) */

        /* expand cache if necessary */
        if (len == cap) {
            size_t newcap = cap * 2;
            if (newcap == 0) newcap = 4;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cap = newcap;
        }

        cache[len] = next;
        unsigned pc_next = popcnt64((uint64_t)next);               /* update frequency for next term */
        freq[pc_next]++;
        len++;
    }

    return cache[n - 1];
}
