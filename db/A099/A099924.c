/*
 * A099924  Self-convolution of Lucas numbers.
 * https://oeis.org/A099924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 5*F(n-1) (n ≥ 2) with a(0) = 4, a(1) =
 *            4. F(k) is the Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 38, u64 n <= 82, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static caches for the sequence and the Fibonacci numbers */
    static seq_t *a_cache = NULL;
    static idx_t a_cap = 0, a_len = 0;
    static seq_t *f_cache = NULL;
    static idx_t f_cap = 0, f_len = 0;

    /* initialise caches on first call */
    if (a_cache == NULL) {
        a_cap = 4;
        a_cache = (seq_t*)calloc(a_cap, sizeof(seq_t));
        a_cache[0] = 4;  /* a(0) */
        a_cache[1] = 4;  /* a(1) */
        a_cache[2] = 13; /* a(2) */
        a_cache[3] = 22; /* a(3) */
        a_len   = 4;

        f_cap = 2;
        f_cache = (seq_t*)malloc(f_cap * sizeof(seq_t));
        f_cache[0] = 0;  /* F(0) */
        f_cache[1] = 1;  /* F(1) */
        f_len   = 2;
    }

    /* grow the a_cache if necessary */
    if ((idx_t)n + 1 > a_cap) {
        size_t new_cap = a_cap * 2;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap   = new_cap;
    }

    /* extend Fibonacci cache to at least index n-1 */
    if ((idx_t)n > 0 && f_len <= (idx_t)n - 1) {
        size_t target = (size_t)n;           /* we need F(n-1) */
        if (f_cap < target + 1) {
            size_t new_cap = f_cap ? f_cap * 2 : 2;
            if (new_cap < target + 1) new_cap = target + 1;
            f_cache = (seq_t*)realloc(f_cache, new_cap * sizeof(seq_t));
            f_cap   = new_cap;
        }
        while (f_len <= (idx_t)n - 1) {
            f_cache[f_len] = f_cache[f_len - 1] + f_cache[f_len - 2];
            ++f_len;
        }
    }

    /* compute a values up to n */
    while (a_len <= n) {
        idx_t i = a_len;
        /* ensure Fibonacci for F(i-1) is available */
        if ((idx_t)i > 1 && f_len <= (idx_t)i - 1) {
            size_t target = (size_t)i;
            if (f_cap < target + 1) {
                size_t new_cap = f_cap ? f_cap * 2 : 2;
                if (new_cap < target + 1) new_cap = target + 1;
                f_cache = (seq_t*)realloc(f_cache, new_cap * sizeof(seq_t));
                f_cap   = new_cap;
            }
            while (f_len <= (idx_t)i - 1) {
                f_cache[f_len] = f_cache[f_len - 1] + f_cache[f_len - 2];
                ++f_len;
            }
        }
        seq_t term = a_cache[i - 1] + a_cache[i - 2] + (seq_t)5 * f_cache[i - 1];
        a_cache[i] = term;
        ++a_len;
    }

    return a_cache[n];
}
