/*
 * A048679  Compressed fibbinary numbers (A003714), with rewrite 0->0, 01->1
 *          applied to their binary expansion.
 * https://oeis.org/A048679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n+1) = min{([a(n)/2]+1)*2^k | ([a(n)/2]+1)*2^k not yet in the
 *            sequence} Initial values: a(0) = 0, a(1) = 1. The sequence is
 *            computed iteratively, storing all values in a dynamic array and
 *            searching linearly for membership (acceptable for n ≤ 10 000)
 * Range:     u8 n <= 54, u16 n <= 2583, u32 n <= 8595, u64 n <= 8595,
 *            u128 n <= 8595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* array holding the sequence */
static size_t cache_size = 0;    /* number of elements currently stored */
static size_t cache_cap   = 0;    /* allocated capacity of `cache` */

/* Append a new element to the cache, growing if necessary. */
static void cache_append(seq_t value) {
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();          /* out of memory */
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_size++] = value;
}

/* Return true iff `value` is already present in the cache. */
static int value_used(seq_t value) {
    for (size_t i = 0; i < cache_size; ++i)
        if (cache[i] == value) return 1;
    return 0;
}

/* Ensure the cache contains at least (idx_t)n+1 elements, computing any
   missing entries. */
static void ensure_upto(idx_t n) {
    size_t target = (size_t)n;
    for (size_t i = cache_size; i <= target; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = 0;
        } else {
            seq_t prev = cache[i - 1];
            seq_t base = (prev >> 1) + 1;   /* floor(prev/2)+1 */
            seq_t candidate = base;
            while (1) {
                if (!value_used(candidate)) {
                    a_i = candidate;
                    break;
                }
                /* avoid infinite loop if shifting would overflow to zero */
                if (candidate == 0) {
                    a_i = candidate;  /* unlikely, but safety */
                    break;
                }
                candidate <<= 1;
            }
        }
        cache_append(a_i);
    }
}

seq_t a(idx_t n) {
    ensure_upto(n);
    return cache[(size_t)n];
}
