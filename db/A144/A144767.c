/*
 * A144767  a(n) = n! - n^(n-6).
 * https://oeis.org/A144767
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = n! - n^(n-6)
 * Range:     i8 none, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 31,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 290 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_u64_idx.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return A144767 at index n (offset 6).  For n < 6 the result is undefined
   but any value is acceptable.  The function works for any n ≥ 6. */
seq_t a(idx_t n) {
    if (n < 6) return 0;            /* value not required below offset */

    /* ---------- factorial cache ------------------------------------- */
    static seq_t *fact_cache = NULL;   /* factorials: fact_cache[i] = i!   */
    static idx_t  cache_cap  = 0;      /* current capacity of fact_cache   */

    /* expand cache if necessary */
    if (n >= cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = old_cap ? old_cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        /* realloc cache */
        seq_t *tmp = seq_realloc(fact_cache, old_cap, new_cap);
        fact_cache = tmp;
        if (!fact_cache) return 0; /* out of memory; caller must handle */

        /* compute missing factorials */
        seq_t val;
        if (old_cap == 0) {
            val = seq_one();          /* 0! = 1 */
            fact_cache[0] = val;
            for (idx_t i = 1; i < new_cap; ++i) {
                val = seq_mul(val, (seq_t)i);
                fact_cache[i] = val;
            }
        } else {
            val = fact_cache[old_cap - 1];
            for (idx_t i = old_cap; i < new_cap; ++i) {
                val = seq_mul(val, (seq_t)i);
                fact_cache[i] = val;
            }
        }
        cache_cap = new_cap;
    }

    /* ---------- compute a(n) ---------------------------------------- */
    seq_t fact = fact_cache[n];
    seq_t power = ipow_u64_idx((uint64_t)n, n - 6);
    return fact - power;
}
