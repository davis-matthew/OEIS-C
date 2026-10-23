/*
 * A173024  Numbers having neither isolated ones nor isolated double ones in
 *          their binary representations.
 * https://oeis.org/A173024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose binary representation has no runs of 1s of
 * length 1 or 2.  The function generates terms on demand by scanning
 * integers and keeping a cache.  a(1) = 0, a(2) = 7, a(3) = 14, ... */

/* The helper to grow a dynamic array. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/append_candidate.h"

/* Static cache holding results.  seq_len is the number of terms
 * currently stored.  next_candidate is the smallest integer yet to
 * be examined. */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static idx_t  next_candidate = 0;

/* Test whether x has no runs of 1s of length 1 or 2. */
static bool good(seq_t x)
{
    while (x) {
        /* Skip leading zeros. */
        while (!(x & 1)) {
            x >>= 1;
            if (!x) return true;   /* only zeros remain */
        }
        /* Count a run of 1s. */
        size_t cnt = 0;
        while (x & 1) {
            ++cnt;
            x >>= 1;
            if (!x) break;
        }
        if (cnt < 3) return false;  /* isolated ones or double ones */
    }
    return true;
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n terms. */
    while (seq_len < n) {
        seq_t cand = (seq_t)next_candidate;
        if (good(cand)) {
            append_candidate(&seq_cache, &seq_len, &seq_cap, cand);
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];   /* 1‑based to 0‑based conversion */
}
