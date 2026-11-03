/*
 * A069693  Triangular numbers with either no internal digits or all internal
 *          digits are 0.
 * https://oeis.org/A069693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th triangular number such that its decimal
 *            representation has no internal digits or all internal digits are
 *            0
 * Range:     u8 n <= 15, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: full, pass, 22 terms through n = 22 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

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

/* Return true if the decimal expansion of x has no internal digits or all
   internal digits are zero.  For x < 10 or x < 100 this is always true. */
static bool internal_ok(seq_t x)
{
    /* Handle single- and two-digit numbers directly. */
    if (x < (seq_t)10ULL || x < (seq_t)100ULL)
        return true;

    /* Remove the last digit. */
    seq_t tmp = x / (seq_t)10ULL;
    /* Strip trailing zeros; after this tmp will contain the first digit
       (or more than one digit if internal digits are not all zero). */
    while (tmp % (seq_t)10ULL == (seq_t)0ULL)
        tmp /= (seq_t)10ULL;

    /* If the remaining part is a single digit, the internal digits were all
       zero (or there were none). */
    return tmp < (seq_t)10ULL;
}

seq_t a(idx_t n)
{
    /* Static cache of qualifying triangular numbers. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static idx_t next_k      = 0;   /* Triangular index to test next. */

    /* Generate until we have at least n terms. */
    if (cache_len < n) {
        /* Allocate initial capacity if necessary. */
        if (cache_cap == 0) {
            cache_cap = 1;
            cache     = seq_realloc(NULL, 0, cache_cap);
        }

        while (cache_len < n) {
            seq_t T = ((seq_t)next_k * ((seq_t)next_k + (seq_t)1)) / (seq_t)2;
            if (internal_ok(T)) {
                /* Grow the array if it is full. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    cache = seq_realloc(cache, cache_cap, new_cap);
                    cache_cap = new_cap;
                }
                cache[cache_len++] = T;
            }
            ++next_k;
        }
    }

    return cache[n - 1];
}
