/*
 * A045191  Numbers whose base-5 representation contains exactly one 0 and no
 *          3's.
 * https://oeis.org/A045191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth integer whose base-5 representation contains
 * exactly one digit 0 and no digit 3.  a(1)=0.  The function builds the
 * sequence lazily and memoises computed values. */
#include <stdbool.h>
#include <stdlib.h>

/* Test whether a number satisfies the base-5 condition.  */
static bool valid(seq_t x) {
    if (x == (seq_t)0)
        return true;                /* "0" has one zero, no three */
    int zero_cnt = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        seq_t d = t % (seq_t)5;
        if (d == (seq_t)0)
            ++zero_cnt;
        else if (d == (seq_t)3)
            return false;           /* forbidden digit */
        t /= (seq_t)5;
    }
    return zero_cnt == 1;
}

/* Returns the nth term of the sequence (1-indexed). */
seq_t a(idx_t n) {
    /* Static cache of computed sequence values: index 1 … computed. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* allocated size (>= computed). */
    static idx_t computed = 0;       /* highest n computed so far. */

    /* Grow cache if necessary. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) return (seq_t)0;   /* out of memory; return 0 as a fallback */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if needed. */
    if (n > computed) {
        seq_t val = (computed == 0) ? (seq_t)0 : cache[computed] + (seq_t)1;
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* Find next value satisfying the condition. */
            while (!valid(val)) ++val;
            cache[i] = val;
            ++val;   /* next search starts after the found value. */
        }
        computed = n;
    }

    return cache[n];
}
