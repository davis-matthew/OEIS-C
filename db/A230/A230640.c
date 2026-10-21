/*
 * A230640  Let M(1)=0 and for n>1, B(n)=(M(ceiling(n/2))+M(floor(n/2))+2)/2,
 *          M(n)=3^B(n)+M(floor(n/2))+1. This sequence gives M(n).
 * https://oeis.org/A230640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   M(1)=0; for n>1: B(n) = (M(⌈n/2⌉)+M(⌊n/2⌋)+2)/2 M(n) = 3^B(n) +
 *            M(⌊n/2⌋) + 1 This implements OEIS sequence A230640
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/big_pow.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure cache has space for indices up to n.  Uninitialized
   entries are set to zero. */
static void ensure_cache(idx_t n) {
    if (n >= cache_size) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        /* Zero the newly allocated portion */
        memset(new_cache + cache_size, 0,
               (new_size - cache_size) * sizeof(seq_t));
        cache = new_cache;
        cache_size = new_size;
    }
}

/* Recursive computation with memoisation. */
static seq_t rec_M(idx_t n) {
    ensure_cache(n);
    if (n == 1) {
        cache[1] = 0;
        return 0;
    }
    if (cache[n] != 0) return cache[n];

    idx_t k = n >> 1;           /* floor(n/2) */
    idx_t c = (n + 1) >> 1;      /* ceil(n/2) */

    seq_t Mk = rec_M(k);
    seq_t Mc = rec_M(c);

    /* B = (Mc + Mk + 2) / 2 */
    seq_t B = (Mc + Mk + 2) >> 1;

    seq_t pow3B = big_pow((seq_t)3, B);
    seq_t Mn = pow3B + Mk + 1;

    cache[n] = Mn;
    return Mn;
}

seq_t a(idx_t n) { return rec_M(n); }
