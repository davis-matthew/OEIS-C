/*
 * A080903  a(1)=1; for n>1, a(n)=a(n-1)+4 if n is already in the sequence,
 *          a(n)=a(n-1)+2 otherwise.
 * https://oeis.org/A080903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 1; for n>1,
 * a(n) = a(n-1) + 4 if n is already in the sequence,
 *       = a(n-1) + 2 otherwise.
 * This function implements the sequence with memoization.
 */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_cache = NULL;      /* array of computed terms */
static idx_t  seq_size   = 0;        /* number of terms stored */
static idx_t  seq_cap    = 0;        /* current array capacity */

/* Ensure seq_cache has at least min_cap entries. */
static void ensure_capacity(idx_t min_cap) {
    if (min_cap <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof *seq_cache);
    if (!tmp) abort();               /* out of memory */
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* Binary search for target in seq_cache[0 .. size-1]. */
static bool contains(idx_t size, seq_t target) {
    idx_t lo = 0, hi = size;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (seq_cache[mid] == target) return true;
        if (seq_cache[mid] < target) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but protect against underflow */

    if (n <= seq_size) {
        /* Value already computed. */
        return seq_cache[n-1];
    }

    /* Ensure space for results up to index n. */
    ensure_capacity(n);

    /* Initialize first term if necessary. */
    if (seq_size == 0) {
        seq_cache[0] = 1;
        seq_size     = 1;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = seq_size + 1; i <= n; ++i) {
        bool inc4 = contains(seq_size, (seq_t)i);
        seq_cache[seq_size] = seq_cache[seq_size-1] + (inc4 ? 4 : 2);
        ++seq_size;
    }

    return seq_cache[n-1];
}
