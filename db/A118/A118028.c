/*
 * A118028  a(0)=1. a(n) = a(n-1) + (smallest integer which is >= n and is
 *          missing from the earlier terms of the sequence).
 * https://oeis.org/A118028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(n)=a(n-1)+(smallest integer>=n missing from
 *            a(0)…a(n-1))
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Static cache for computed terms */
static seq_t *seq_cache = NULL;      /* allocated array of terms      */
static size_t seq_len  = 0;           /* number of terms currently in cache */
static size_t seq_cap  = 0;           /* allocated capacity of the cache */

/* Binary search: return true if val is present in arr[0..len-1] */
static bool contains(const seq_t *arr, size_t len, seq_t val) {
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] == val) return true;
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Append a new term to the cache, expanding the array if necessary */
static void append_term(seq_t value) {
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = value;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) {
        if (seq_len == 0) {
            /* Initialize a(0) */
            append_term((seq_t)1);
        }
        return seq_cache[0];
    }

    /* Compute terms up to index n */
    while (seq_len <= (size_t)n) {
        idx_t cur_idx = (idx_t)seq_len;      /* current index to compute */
        /* Find smallest integer >= cur_idx that is not in seq_cache[0..cur_idx-1] */
        seq_t candidate = (seq_t)cur_idx;
        while (contains(seq_cache, seq_len, candidate))
            ++candidate;

        /* New term = previous term + candidate */
        seq_t new_val = seq_cache[seq_len - 1] + candidate;
        append_term(new_val);
    }

    /* Return the requested term */
    return seq_cache[(size_t)n];
}
