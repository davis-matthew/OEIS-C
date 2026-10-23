/*
 * A020718  Pisot sequences E(6,10), P(6,10).
 * https://oeis.org/A020718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=6, a(1)=10, a(2)=17, a(3)=29, a(4)=49, a(5)=83,
 *            a(n)=2*a(n-1)-a(n-3)+a(n-5)-a(n-6) for n>=6
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 79, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A020718 (Pisot sequence E(6,10)). */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>   /* for abort() */

/* Static cache of computed values. */
static seq_t *seq_cache = NULL;
static size_t seq_cache_size = 0;
static size_t seq_cache_cap  = 0;

/* Ensure the cache can hold indices up to 'need' (inclusive). */
static inline void ensure_cache_capacity(size_t need) {
    if (need < seq_cache_cap)
        return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 32;
    while (new_cap <= need)
        new_cap <<= 1;
    /* realloc may fail; in that case abort because we cannot continue. */
    seq_t *ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!ptr)
        abort();
    seq_cache = ptr;
    seq_cache_cap = new_cap;
}

/* Compute a(idx_t n). */
seq_t a(idx_t n) {
    /* Initial values for n < 6. */
    if (n < 6) {
        static const seq_t init[6] = {6, 10, 17, 29, 49, 83};
        return init[(size_t)n];
    }

    /* Make sure the cache holds at least the first six values. */
    if (seq_cache_size < 6) {
        ensure_cache_capacity(5);
        seq_cache[0] = 6;
        seq_cache[1] = 10;
        seq_cache[2] = 17;
        seq_cache[3] = 29;
        seq_cache[4] = 49;
        seq_cache[5] = 83;
        seq_cache_size = 6;
    }

    /* Compute any missing terms up to 'n'. */
    size_t up_to = (size_t)n;
    if (seq_cache_size <= up_to) {
        ensure_cache_capacity(up_to);
        for (size_t i = seq_cache_size; i <= up_to; ++i) {
            seq_t a1 = seq_cache[i-1];
            seq_t a3 = seq_cache[i-3];
            seq_t a5 = seq_cache[i-5];
            seq_t a6 = seq_cache[i-6];
            /* Guarantee that intermediate results fit in seq_t. */
            seq_t term = 2*a1;
            term -= a3;
            term += a5;
            term -= a6;
            seq_cache[i] = term;
        }
        seq_cache_size = up_to + 1;
    }

    return seq_cache[up_to];
}
