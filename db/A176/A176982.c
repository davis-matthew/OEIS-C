/*
 * A176982  Sequence defined by the recursion a(n) =
 *          (1/2)*((1-signum(abs(b(n))-n))*b(n)+(1+signum(abs(b(n))-n))*a(n-1)),
 *          with a(1)=1 and b(n)=1+a(n-1-(n mod a(n-1)))-(-1)^n*a(n-1).
 * https://oeis.org/A176982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) =
 *            (1/2)*((1-signum(abs(b(n))-n))*b(n)+(1+signum(abs(b(n))-n))*a(n-1)),
 *            with a(1)=1 and b(n)=1+a(n-1-(n mod a(n-1)))-(-1)^n*a(n-1)
 * Range:     i8 n <= 151, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation keeps a static memoisation table so that
    a(n) can be obtained in any order.  It works for all the
    integer types supported by the build (int32_t, int64_t, __int128
    or an arbitrary‑precision class).  */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;    /* 1‑based cache, cache[0] unused */
static idx_t cache_len = 0;    /* highest index computed so far */
static size_t cache_cap = 0;   /* size of the allocated array */

/*  Compute a(n) for n >= 1.  */
seq_t a(idx_t n)
{
    /*  For safety – the OEIS sequence is defined only for n >= 1. */
    if (n < 1) {
        return (seq_t)0;
    }

    /*  First‑time initialisation of the cache.  */
    if (cache == NULL) {
        cache_cap = 64;                      /* arbitrary initial size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;         /* allocation failed */
        cache[0] = (seq_t)0;                 /* unused element */
        cache[1] = (seq_t)1;                 /* a(1)=1 */
        cache_len = 1;
    }

    /*  Grow the cache if we need to compute more terms.  */
    if (n > cache_len) {
        if ((size_t)n + 1 > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;      /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];          /* a(i-1) */

            /*  Compute i mod a(i-1) safely:   */
            seq_t i_seq = (seq_t)i;
            idx_t mod;
            if (prev == (seq_t)0) {
                /*  Should never happen; guard against divide‑by‑zero.  */
                mod = 0;
            } else {
                seq_t rem = i_seq % prev;      /* remainder: 0 <= rem < prev */
                mod = (idx_t)rem;              /* valid because rem < i <= 2^64-1 */
            }

            /*  Index for a(i-1 - (i mod a(i-1)))  */
            idx_t idx = (idx_t)(i - 1) - mod;
            if (idx < 1) idx = 1;              /* safety guard – a(0) not defined */

            seq_t idx_val = cache[idx];

            /*  (-1)^i  */
            seq_t factor = (i % 2 == 0) ? (seq_t)1 : (seq_t)-1;

            /*  b(i) = 1 + a(idx) - (-1)^i * a(i-1)  */
            seq_t b = (seq_t)1 + idx_val - factor * prev;

            /*  abs(b) - i  */
            seq_t abs_b = (b >= (seq_t)0) ? b : -b;
            seq_t diff  = abs_b - (seq_t)i;

            /*  signum(diff)  */
            int sgn = (diff > (seq_t)0) - (diff < (seq_t)0);

            /*  a(i) = ((1-sgn)*b + (1+sgn)*prev) / 2  */
            seq_t a_val = ((seq_t)(1 - sgn) * b + (seq_t)(1 + sgn) * prev) / (seq_t)2;

            cache[i] = a_val;
        }
        cache_len = n;
    }

    return cache[n];
}
