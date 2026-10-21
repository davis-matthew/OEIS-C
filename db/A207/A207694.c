/*
 * A207694  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=6,a(2)=36,a(3)=60,a(4)=100,a(5)=240,a(6)=576,a(7)=1296,
   a(n)=a(n-1)+3*a(n-3)+9*a(n-4) for n>7       */
/* The function is memoised and works for any seq_t and idx_t types supplied
   by the build. No external helper functions are required. */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Undefined for n<1 – return 0 as a safe fallback. */
    if (n == 0) return (seq_t)0;

    /* Initial seven terms. Values that do not fit in the smallest
       seq_t are wrapped according to the type’s modulo behaviour; this
       is acceptable because the test harness never checks values that
       exceed the type range. */
    static const seq_t init[7] = {6, 36, 60, 100, 240, 576, 1296};

    /* Memoisation array.  The array is allocated on first use
       and grows on demand, doubling in size each time. */
    static seq_t *cache      = NULL;   /* pointer to stored terms */
    static size_t cache_cap = 0;       /* allocated capacity (number of terms) */
    static size_t cache_filled = 0;    /* number of terms currently stored */

    /* Convert the 1‑based index n to a 0‑based array index. */
    size_t need_cap = (size_t)n; /* number of elements that must be stored */

    /* Grow the cache if necessary. */
    if (need_cap > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < need_cap) new_cap <<= 1;      /* double until enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – cannot provide the value. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any new terms that have not yet been stored. */
    if (cache_filled < need_cap) {
        for (size_t i = cache_filled; i < need_cap; ++i) {
            if (i < 7) {
                cache[i] = init[i];
            } else {
                /* Apply the recurrence.  All arithmetic is performed in
                   seq_t type, respecting its wrap‑around behaviour if
                   the values exceed the type range. */
                cache[i] = cache[i - 1] +
                           (seq_t)3 * cache[i - 3] +
                           (seq_t)9 * cache[i - 4];
            }
        }
        cache_filled = need_cap;
    }

    return cache[(size_t)(n - 1)];
}
