/*
 * A158026  Fibonacci numbers f such that all the digits needed to write the
 *          consecutive Fibonacci numbers from 0 to f fill exactly a square
 *          (no holes, no overlaps).
 * https://oeis.org/A158026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   digit_len, seq_alloc, seq_free, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search over Fibonacci numbers, recording cumulative digit counts
   and checking when the count is a perfect square.  The function caches
   found terms.  It handles any seq_t and idx_t type supplied by the build. */
#include <stddef.h>
#include <stdint.h>

#include "../utils/digit_len.h"      /* int digit_len(seq_t x) */
#include "../utils/seq_mul.h"        /* seq_t seq_mul(seq_t a, seq_t b) */
#include "../utils/seq_alloc.h"      /* seq_t *seq_alloc(size_t) */
#include "../utils/seq_realloc.h"    /* seq_t *seq_realloc(seq_t *old, size_t, size_t) */
#include "../utils/seq_free.h"       /* void seq_free(seq_t *p) */

seq_t a(idx_t n)
{
    /* cache of sequence values */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* state kept between calls */
    static seq_t sum = 0;          /* cumulative digit count */
    static seq_t t   = 0;          /* current square root candidate   */
    static seq_t t_sq = 0;         /* t * t                          */
    static seq_t f_prev = 0;       /* current Fibonacci number Fi    */
    static seq_t f_curr = 1;       /* next Fibonacci number Fi+1     */

    /* produce terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* 1. add digit count of f_prev */
        int digits = digit_len(f_prev);
        sum = sum + (seq_t)digits;

        /* 2. increase t until t*t >= sum */
        while (sum > t_sq) {
            t = t + (seq_t)1;
            t_sq = seq_mul(t, t);
        }

        /* 3. if sum == t_sq we have a new term */
        if (sum == t_sq) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!cache) {/* first allocation may fail */ }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = f_prev;
        }

        /* 4. next Fibonacci number */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }

    return cache[n - 1];
}
