/*
 * A044734  Numbers n such that string 2,1 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose decimal representation contains * the
 *            substring "21" but whose successor does not. * Computed by
 *            brute‑force search with memoisation
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: returns true iff decimal digits of x contain the substring "21" */
static bool has_21(seq_t x)
{
    seq_t prev = (seq_t)10;            /* sentinel unequal to any digit */
    while (x > (seq_t)0) {
        seq_t cur = x % (seq_t)10;
        if (cur == (seq_t)2 && prev == (seq_t)1)
            return true;
        prev = cur;
        x /= (seq_t)10;
    }
    return false;
}

/* Return the nth term (1‑based index) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for 0; return 0 as a guard */
        return (seq_t)0;

    /* Static state for memoisation across calls. */
    static seq_t *cache     = NULL;  /* array of found terms          */
    static size_t cache_cap = 0;     /* capacity of the array        */
    static size_t cache_len = 0;     /* number of terms stored       */
    static idx_t  next_check = 1;    /* next integer to test         */
    static bool   init = false;      /* initialisation flag          */

    if (!init) {
        /* Allocate initial space for 64 terms. */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_check = 1;
        init = true;
    }

    /* Keep generating until we have at least n terms. */
    while (cache_len < n) {
        idx_t m = next_check;
        while (1) {
            seq_t val = (seq_t)m;
            if (has_21(val) && !has_21(val + (seq_t)1)) {
                /* Store the value. */
                if (cache_len >= cache_cap) {
                    cache_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  cache_cap * sizeof(seq_t));
                    if (!tmp)            /* allocation failed */
                        return (seq_t)0;
                    cache = tmp;
                }
                cache[cache_len++] = val;
                next_check = m + 1;
                break;                /* found next term */
            }
            ++m;
        }
    }

    return cache[n - 1];
}
