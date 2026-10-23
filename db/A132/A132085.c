/*
 * A132085  Number of partitions of n into distinct parts such that (u+1)^2 <=
 *          v for all pairs (u,v) of parts with u<v.
 * https://oeis.org/A132085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts such * that
 *            for every pair of parts (u,v) with u<v we have (u+1)^2 <= v. *
 *            Computed recursively with memoization exploiting the rapid
 *            growth of * allowed next parts
 * Range:     u8 n <= 2654, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Cache entry for memoization */
typedef struct {
    bool used;
    idx_t rem;
    idx_t min_part;
    seq_t val;
} cache_entry_t;

/* Size of the hash table (must be a power of two for masking) */
#define CACHE_CAP 262144
#define CACHE_MASK (CACHE_CAP - 1)

static cache_entry_t cache[CACHE_CAP];

/* Hash function mixing remainder and minimum part into a 64-bit key */
static uint64_t cache_hash(idx_t rem, idx_t min_part) {
    return ((uint64_t)rem << 32) ^ (uint64_t)min_part;
}

/* Find existing entry or allocate a new one for (rem,min_part) */
static cache_entry_t *cache_lookup(idx_t rem, idx_t min_part, bool *found) {
    uint64_t h = cache_hash(rem, min_part);
    for (uint64_t i = h & CACHE_MASK; ; i = (i + 1) & CACHE_MASK) {
        if (!cache[i].used) {          /* empty slot – insert */
            cache[i].used = true;
            cache[i].rem = rem;
            cache[i].min_part = min_part;
            if (found) *found = false;
            return &cache[i];
        }
        if (cache[i].rem == rem && cache[i].min_part == min_part) {
            if (found) *found = true;
            return &cache[i];
        }
    }
}

/* Recursive helper counting partitions of 'rem' with parts >= 'min_part' */
static seq_t count_partitions(idx_t rem, idx_t min_part) {
    if (rem == 0) return (seq_t)1;            /* empty partition */
    if (min_part > rem) return (seq_t)0;      /* impossible */

    bool found;
    cache_entry_t *entry = cache_lookup(rem, min_part, &found);
    if (found) return entry->val;             /* memoized result */

    seq_t total = (seq_t)0;
    for (idx_t k = min_part; k <= rem; ++k) {
        if (k == rem) {                       /* single part */
            total += (seq_t)1;
            break;
        }
        idx_t remaining = rem - k;
        idx_t next_min = (idx_t)(k + 1) * (k + 1);   /* (k+1)^2 */
        if (remaining < next_min) continue;          /* no further parts possible */
        total += count_partitions(remaining, next_min);
    }

    entry->val = total;                       /* store memoized value */
    return total;
}

/* Public function as required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;              /* not defined for n<1 */
    return count_partitions(n, (idx_t)1);
}
