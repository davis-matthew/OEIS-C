/*
 * A224753  a(2)=2; thereafter a(n) = smallest number m such that a(n-1)+m =
 *          (a(n-1) followed by the leading digit of m).
 * https://oeis.org/A224753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   digit_len, pow10, seq_alloc, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A224753:  a(1)=2; for n>1, a(n) is the smallest m such that
 *  a(n-1)+m = (a(n-1) followed by the leading digit of m).
 *  Equivalently, m = 9*a(n-1)+d where d is the leading digit of m. */

#include <stddef.h>                 /* for size_t */
#include "../utils/digit_len.h"     /* int digit_len(seq_t x); */
#include "../utils/pow10.h"         /* seq_t pow10(idx_t e); */
#include "../utils/seq_alloc.h"     /* seq_t* seq_alloc(size_t); */
#include "../utils/seq_realloc.h"   /* seq_t* seq_realloc(seq_t*, size_t, size_t); */
#include "../utils/seq_mul.h"       /* seq_t seq_mul(seq_t a, seq_t b); */

/* Return the leading decimal digit of a positive x. */
static seq_t leading_digit(seq_t x)
{
    if (x == 0) return 0;               /* safety – never happens for x>0 */
    int len = digit_len(x);              /* number of decimal digits of x */
    seq_t p10 = pow10((idx_t)(len - 1)); /* 10^(len-1) */
    return x / p10;
}

/* Cached sequence. */
static seq_t *cache     = NULL;   /* array of values      */
static size_t cache_cap = 0;      /* allocated capacity   */
static size_t cache_len = 0;      /* number of terms stored */

/* Ensure that cache holds at least new_len elements. */
static void ensure_cache_size(size_t new_len)
{
    if (new_len <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < new_len) new_cap *= 2;

    if (cache == NULL) {
        cache = seq_alloc(new_cap);
    } else {
        cache = seq_realloc(cache, cache_cap, new_cap);
    }
    cache_cap = new_cap;
}

/* Compute a(n).  Cache previous values for fast repeated calls. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1 */

    size_t target = (size_t)n;      /* target index to compute */
    ensure_cache_size(target);

    if (cache_len == 0) {           /* initialise a(1) */
        cache[0] = (seq_t)2;
        cache_len = 1;
    }

    /* Extend cache up to target. */
    if (cache_len < target) {
        for (size_t i = cache_len; i < target; ++i) {
            seq_t prev = cache[i - 1];
            seq_t nine_prev = seq_mul(prev, (seq_t)9);   /* 9*a(n-1) */

            seq_t d = 0;
            for (int dd = 1; dd <= 9; ++dd) {
                seq_t cand = nine_prev + (seq_t)dd;     /* m = 9*prev + d */
                if (leading_digit(cand) == (seq_t)dd) {
                    d = (seq_t)dd;
                    break;
                }
            }
            cache[i] = nine_prev + d;  /* store a(n) */
        }
        cache_len = target;
    }

    return cache[n - 1];
}
