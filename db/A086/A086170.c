/*
 * A086170  a(1)=1; a(n)=a(n-1)+1 if n is in the sequence; a(n)=a(n-1)+2 if n
 *          and (n-1) are not in the sequence; a(n)=a(n-1)+3 if n is not in
 *          the sequence but (n-1) is in the sequence.
 * https://oeis.org/A086170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>=2, a(n)=a(n-1)+1 if n is already a term of the
 *            sequence, a(n)=a(n-1)+2 if n and (n-1) are not terms of the
 *            sequence, a(n)=a(n-1)+3 if n is not a term but (n-1) is a term.
 *            The sequence is computed iteratively with membership information
 *            stored in a dynamic boolean array
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Helper to resize and zero the boolean presence array */
static void ensure_present_capacity(bool **present, size_t *cap, size_t idx)
{
    if (idx < *cap) return;
    size_t new_cap = (*cap > 0) ? *cap * 2 : 64;
    while (new_cap <= idx) new_cap <<= 1;
    bool *tmp = (bool *)realloc(*present, new_cap * sizeof(bool));
    /* Zero any new entries */
    for (size_t i = *cap; i < new_cap; ++i) tmp[i] = false;
    *present = tmp;
    *cap = new_cap;
}

/* Record that a value appears in the sequence */
static void record_presence(bool *present, size_t cap, seq_t val)
{
    if (val > SIZE_MAX) return;              /* value too large for index */
    size_t idx = (size_t)val;
    if (idx >= cap) return;                   /* we didn't enlarge to the needed size */
    present[idx] = true;
}

/* Helper to resize the cache that stores computed a(n) values */
static void ensure_cache_capacity(seq_t **cache, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap > 0) ? *cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    *cache = mem_resize(seq_t, *cache, new_cap);
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_cap   = 0;
    static size_t cache_size = 0;

    static bool *present  = NULL;
    static size_t present_cap = 0;

    if (n == 0) return (seq_t)0;   /* safeguard, not part of OEIS definition */

    /* Initialisation for the very first call */
    if (cache_size == 0) {
        ensure_cache_capacity(&cache, &cache_cap, 1);
        cache[0] = (seq_t)1;
        cache_size = 1;

        present_cap = 64;
        present = (bool *)calloc(present_cap, sizeof(bool));
        record_presence(present, present_cap, (seq_t)1);
    }

    /* Compute all values up to the requested n */
    while (cache_size < (size_t)n) {
        idx_t k = (idx_t)cache_size + 1;   /* next index to compute */

        bool k_in_seq   = false;
        bool km1_in_seq = false;

        if (k < present_cap && present[(size_t)k])   k_in_seq = true;
        if ((k > 0) && ((k-1) < present_cap) && present[(size_t)(k-1)]) km1_in_seq = true;

        seq_t delta;
        if (k_in_seq)                 delta = (seq_t)1;
        else if (!k_in_seq && !km1_in_seq) delta = (seq_t)2;
        else                          delta = (seq_t)3;

        seq_t prev = cache[cache_size - 1];
        seq_t val  = prev + delta;

        /* Record the new value in the presence array if it fits */
        if (val <= SIZE_MAX) {
            size_t idx_val = (size_t)val;
            if (idx_val >= present_cap) {
                ensure_present_capacity(&present, &present_cap, idx_val);
            }
            record_presence(present, present_cap, val);
        }

        /* Store the new term */
        ensure_cache_capacity(&cache, &cache_cap, cache_size + 1);
        cache[cache_size] = val;
        cache_size++;
    }

    return cache[n - 1];
}
