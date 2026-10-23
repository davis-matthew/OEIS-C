/*
 * A044413  Numbers n such that string 8,1 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044413: numbers n such that the decimal string "81"
   occurs in n but does not occur in n-1.  The value a(n) is
   found by a simple brute‑force scan keeping a cache of already
   found terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* static state: cache of terms, last tested value, etc. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;     /* number of terms cached */
    static size_t cache_cap   = 0;     /* allocated capacity */
    static seq_t last        = 0;      /* last integer examined */
    static bool  prev_has81  = false;  /* whether previous integer had "81" */

    if (n == 0)                     /* undefined for the offset */
        return 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();     /* failure is fatal */
        cache_size = 0;
        last        = 0;
        prev_has81  = false;
    }

    /* generate terms until we have at least n terms */
    while (cache_size < (size_t)n) {
        /* grow cache if needed */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* next number to examine */
        last = last + 1;

        /* check whether last contains the substring "81" */
        bool has81 = false;
        seq_t t = last;
        int prev_digit = -1;
        while (t != 0) {
            unsigned int digit = (unsigned int)(t % 10);
            if (digit == 8 && prev_digit == 1) {
                has81 = true;
                break;
            }
            prev_digit = digit;
            t = t / 10;
        }

        /* record term if it satisfies the definition */
        if (has81 && !prev_has81) {
            cache[cache_size++] = last;
        }

        /* update flag for next iteration */
        prev_has81 = has81;
    }

    return cache[n - 1];
}
