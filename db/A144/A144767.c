/*
 * A144767  a(n) = n! - n^(n-6).
 * https://oeis.org/A144767
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = n! - n^(n-6)
 * Range:     i8 none, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 31,
 *            big any n
 * Helpers:   pow_uint, seq_mul, seq_one, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_realloc.h"

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
    seq_t power = pow_uint((uint64_t)n, n - 6);
    return fact - power;
}
