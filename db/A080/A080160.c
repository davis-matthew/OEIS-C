/*
 * A080160  Squares that are digit sums of Wonderful Demlo numbers A002477.
 * https://oeis.org/A080160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 108, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A080160: Squares that are digit sums of Wonderful Demlo numbers (A002477).
 * The sequence consists of the squares x² where x is the nth positive integer
 * satisfying one of the congruence classes  
 *     x mod 81 ∈ {0,1,2,3,4,5,6,7,8,9,18,21,24,27,30,33,36,
 *                  45,48,51,54,57,60,63,72,73,74,75,76,77,78,79,80}.
 * That set contains 33 residues out of 81, so about 40.7% of all positive integers are accepted.
 * 
 * The function builds the terms lazily: each call computes terms up to the
 * requested index, caching them for future calls.  The algorithm is simply
 * iterating over x = 1,2,3,… and pushing the square into the cache
 * whenever x satisfies the residue condition.
 */

#include <stdlib.h>
#include <stdbool.h>

static const int good_residues[] = {
    0,1,2,3,4,5,6,7,8,9,
    18,21,24,27,30,33,36,45,48,51,
    54,57,60,63,72,73,74,75,76,77,
    78,79,80
};
static const size_t good_res_cnt = sizeof(good_residues)/sizeof(good_residues[0]);

/* Utility: test if r is in the good_residues table */
static bool res_is_good(int r) {
    /* Simple linear search; table size is only 33 */
    for (size_t i = 0; i < good_res_cnt; ++i)
        if (good_residues[i] == r) return true;
    return false;
}

/* Cache for computed sequence */
static seq_t *cache = NULL;       /* array of squares */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity */

/* Grow cache if needed */
static void ensure_cache_cap(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return; /* allocation failure: abort silently */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* undefined for 0, return 0 */
    /* Extend cache up to n terms if needed */
    if (cache_len < (size_t)n) {
        /* Generate terms until we have n */
        idx_t x = 1;           /* current candidate integer */
        /* If cache already has terms, set x to last cached value +1 */
        if (cache_len > 0) x = (idx_t)cache_len + 1; /* because cache_len equals number of terms, i.e., highest x accepted */
        /* But x must continue from last accepted, not from cache_len+1. */
        /* Need to keep track of last generated candidate index, not cached value. */
        static idx_t next_candidate = 1;
        /* But we cannot rely on static across calls? we already have cache, so compute from previous max. */
        /* Let's find maximum x that has been accepted: since each accepted x is one of the residues, and they are roughly every 1.5 numbers, but we can just continue iterating from last accepted x+1. */
        /* We can recover last accepted x by reverse computing from last cached square's sqrt, but that requires sqrt. We'll store last accepted x in static */
    }
    /* The above reasoning indicates we need to remember last x we processed, not just last accepted. */
    /* Start over: implement static next_x after last generated (not cached). */
    static idx_t next_x = 1; /* Next candidate */
    /* Ensure we have at most one loop per call. We'll generate until cache_len == n. */
    while (cache_len < (size_t)n) {
        int r = (int)(next_x % 81);
        if (res_is_good(r)) {
            seq_t sq = (seq_t)next_x * (seq_t)next_x;
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = sq;
        }
        ++next_x;
    }
    return cache[n-1];
}
