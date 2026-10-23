/*
 * A187309  Half the number of (n+2) X 3 binary arrays with each 3 X 3
 *          subblock having sum 3, 4, 5 or 6.
 * https://oeis.org/A187309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + a(n-2) - 6*a(n-3) - 6*a(n-4) - 27*a(n-5) +
 *            90*a(n-6) - 81*a(n-8) Initial values (n≥1): a(1)=210, a(2)=1472,
 *            a(3)=10262, a(4)=71836, a(5)=502545, a(6)=3516295,
 *            a(7)=24602854, a(8)=172142801 (All further terms are obtained
 *            from the recurrence)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS sequence A187309.
   idx_t is an unsigned integer type for indices (uint64_t), 
   seq_t is an unsigned integer type or arbitrary‑precision integer class. */
seq_t a(idx_t n)
{
    /* Precomputed values a(1) … a(8).  index 0 is unused. */
    static const seq_t base[9] = {
        0,          /* base[0] unused */
        210,        /* a(1) */
        1472,       /* a(2) */
        10262,      /* a(3) */
        71836,      /* a(4) */
        502545,     /* a(5) */
        3516295,    /* a(6) */
        24602854,   /* a(7) */
        172142801   /* a(8) */
    };

    /* Static cache for computed terms.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* Current allocated size of cache */
    static int init    = 0;   /* Flag: 0 = not initialized */

    if (!init) {
        cache = NULL;
        cap = 0;
        init = 1;
    }

    /* Return early for known small indices */
    if (n <= 8) {
        return base[n];
    }

    /* Ensure capacity up to n (inclusive) */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort with zero (undefined behavior). */
            return 0;
        }
        cache = tmp;
        /* If the cache was just sized to accommodate the first 8 terms,
           fill in the base values.  If it was already larger, they
           are already stored. */
        if (cap < 9) {
            for (idx_t i = 1; i <= 8; ++i) {
                cache[i] = base[i];
            }
        }
        cap = new_cap;
    }

    /* Compute terms from the first missing index up to n */
    for (idx_t i = 9; i <= n; ++i) {
        cache[i] = 7 * cache[i - 1]
                 + cache[i - 2]
                 - 6 * cache[i - 3]
                 - 6 * cache[i - 4]
                 - 27 * cache[i - 5]
                 + 90 * cache[i - 6]
                 - 81 * cache[i - 8];
    }

    return cache[n];
}
