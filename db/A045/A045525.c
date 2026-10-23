/*
 * A045525  Numbers k such that k! has initial digit '6'.
 * https://oeis.org/A045525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 4407, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A045525: Numbers k such that k! has initial digit 6
 * Formula: a(n) = the n-th integer k for which the first digit of k! is 6
 */

#include <stdlib.h>
#include <math.h>
/* include any helper if needed, but we use standard realloc */
static seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap);

seq_t a(idx_t n) {
    /* Static state for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    static idx_t next_k = 1;      /* next k to test */
    static long double logsum = 0.0L;  /* Σ log10(i) for i=1..next_k-1 */

    /* Expand cache until we have at least n terms */
    while (cache_size < n) {
        /* add log10(next_k) to cumulative sum */
        logsum += log10l((long double)next_k);

        /* check first digit of next_k! */
        long double frac = logsum - floorl(logsum);
        int first = (int)powl(10.0L, frac);

        if (first == 6) {
            /* ensure capacity */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) return 0; /* out of memory, undefined behaviour */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}

/* Simple implementation using realloc that matches the expected signature */
seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) {
    (void)old_cap; /* unused */
    /* Note: realloc may return NULL on failure */
    return (seq_t *)realloc(old, new_cap * sizeof(seq_t));
}
