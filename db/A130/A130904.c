/*
 * A130904  Numbers k such that the trajectory of the map k -> (k + lpf(k)) /
 *          2 reaches 3, where lpf(k) is the least prime factor of k
 *          (A020639).
 * https://oeis.org/A130904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: for each integer k >= 3, follow the map
 * k -> (k + lpf(k)) / 2 until it reaches 3 (accept) or a
 * fixed point other than 3 (reject).  The sequence a(n) is
 * the n-th integer that eventually reaches 3. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/lpf.h"

static seq_t *cache = NULL;          /* cache of sequence terms    */
static idx_t cache_len  = 0;          /* number of terms cached     */
static idx_t cache_cap  = 0;          /* allocated capacity         */
static idx_t next_candidate = 3;      /* next integer to test       */

/* Return true iff the trajectory of start eventually reaches 3. */
static bool reaches3(idx_t start)
{
    idx_t cur = start;
    /* A generous bound; the trajectory always terminates well
     * before 500 iterations for any integer that eventually
     * reaches 3. */
    for (int i = 0; i < 500; ++i) {
        if (cur == 3) return true;
        if (cur == 2) return false;               /* fixed point 2 */
        idx_t l = lpf(cur);                       /* least prime factor */
        if (l == cur) return false;               /* prime ≠ 3 fixed point */
        cur = (cur + l) / 2;
    }
    return false; /* theoretically unreachable */
}

seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        if (reaches3(next_candidate)) {
            /* resize if necessary */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
