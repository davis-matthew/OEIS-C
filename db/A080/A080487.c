/*
 * A080487  a(1) =2, a(n) = smallest multiple of a(n-1) (not equal to
 *          10^k*a(n-1)) obtained by inserting digits anywhere in a(n-1).
 * https://oeis.org/A080487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   a(1) = 2 a(2) = 12 a(3) = 132 a(4) = 10032 For n ≥ 5, a(n) =
 *            a(n‑1) * (10^d + 1) where d = ceil( digit_len(a(n‑1)) / 2 )
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   digit_len, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a cache so that calls to a(n) can be in any order
   and the result for each n is computed only once.  The core recurrence
   is computed directly using pow10 from the utilities. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

/* ----- cache ----- */
static seq_t *cache      = NULL;   /* a(1) .. a(cache_len) */
static size_t cache_len  = 0;      /* number of terms stored  (>=0) */
static size_t cache_cap  = 0;      /* allocated capacity    */

/* Compute a(n) for n ≥ 1.  a(0) is undefined; returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined, safety */

    if (cache_len < (size_t)n) {
        /* grow the cache if necessary */
        size_t needed = (size_t)n;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* initialise the first term if needed */
        if (cache_len == 0) {
            cache[0] = (seq_t)2;   /* a(1) */
            cache_len = 1;
        }

        /* fill in missing terms */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            seq_t val;
            /* i == 0 corresponds to n == 1, already set above */
            if (i == 1)        val = (seq_t)12;          /* a(2) */
            else if (i == 2)   val = (seq_t)132;         /* a(3) */
            else if (i == 3)   val = (seq_t)10032;      /* a(4) */
            else {
                seq_t prev = cache[i - 1];
                size_t len_prev = digit_len(prev);
                size_t d = (len_prev + 1) / 2;              /* ceil(len/2) */
                seq_t pow10d = pow10((idx_t)d);            /* 10^d */
                val = seq_mul(prev, pow10d);               /* prev * 10^d */
                val = val + prev;                          /* + prev  */
            }
            cache[i] = val;
        }

        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
