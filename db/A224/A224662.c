/*
 * A224662  Number of n X n 0..5 matrices with each 2X2 subblock idempotent.
 * https://oeis.org/A224662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 12*a(n-2) + 5*a(n-3) + 12*a(n-4) - 12*a(n-5) -
 *            3*a(n-6) + 6*a(n-7) - a(n-9) for n > 10, with initial values
 *            a(1)=6, a(2)=24, a(3)=128, a(4)=222, a(5)=500, a(6)=956,
 *            a(7)=1876, a(8)=3538, a(9)=6622, a(10)=12200
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"     /* declares seq_alloc(size_t)   */
#include "../utils/seq_realloc.h"   /* declares seq_realloc(seq_t*,size_t,size_t) */

static const seq_t init_vals[11] = {
    (seq_t)0,   /* unused 0‑th position for easier indexing */
    (seq_t)6,   /* a(1)  */
    (seq_t)24,  /* a(2)  */
    (seq_t)128, /* a(3)  */
    (seq_t)222, /* a(4)  */
    (seq_t)500, /* a(5)  */
    (seq_t)956, /* a(6)  */
    (seq_t)1876,/* a(7)  */
    (seq_t)3538,/* a(8)  */
    (seq_t)6622,/* a(9)  */
    (seq_t)12200/* a(10)*/
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array holding computed terms */
    static size_t cap   = 0;       /* current allocation size (>= n+1) */
    static idx_t  max   = 0;       /* highest index that has been computed */

    /* ------------------------------------------------------------ */
    /* 1. Ensure that the cache is large enough for the requested n */
    /* ------------------------------------------------------------ */
    if (cache == NULL) {
        /* at first allocation we need at least n+1 slots */
        cap = 16;
        while (cap <= (size_t)n) cap <<= 1;
        cache = seq_alloc(cap);           /* strict allocation */
    } else if (cap <= (size_t)n) {
        size_t old_cap = cap;
        while (cap <= (size_t)n) cap <<= 1;
        cache = seq_realloc(cache, old_cap, cap);
    }

    /* ------------------------------------------------------------ */
    /* 2. If we do not yet have the first 10 terms, insert them */
    /* ------------------------------------------------------------ */
    if (max < (idx_t)10) {
        for (idx_t i = max + 1; i <= (idx_t)10; ++i) {
            cache[i] = init_vals[(size_t)i];
        }
        if (max < (idx_t)10) max = (idx_t)10;
    }

    /* ------------------------------------------------------------ */
    /* 3. Directly return a(n) for the first 10 indices            */
    /* ------------------------------------------------------------ */
    if (n <= (idx_t)10) {
        return init_vals[(size_t)n];
    }

    /* ------------------------------------------------------------ */
    /* 4. Compute successive terms using the given recurrence     */
    /* ------------------------------------------------------------ */
    for (idx_t i = max + 1; i <= n; ++i) {
        /* recurrence requires i-1 .. i-9 ; these indices always exist
           because max >= 10 and i increases sequentially */
        seq_t t1 = (seq_t)6   * cache[(size_t)(i - 1)];
        seq_t t2 = (seq_t)12  * cache[(size_t)(i - 2)];
        seq_t t3 = (seq_t)5   * cache[(size_t)(i - 3)];
        seq_t t4 = (seq_t)12  * cache[(size_t)(i - 4)];
        seq_t t5 = (seq_t)12  * cache[(size_t)(i - 5)];
        seq_t t6 = (seq_t)3   * cache[(size_t)(i - 6)];
        seq_t t7 = (seq_t)6   * cache[(size_t)(i - 7)];
        seq_t t9 =          cache[(size_t)(i - 9)];

        cache[(size_t)i] = t1 - t2 + t3 + t4 - t5 - t6 + t7 - t9;
    }

    max = n;
    return cache[(size_t)n];
}
