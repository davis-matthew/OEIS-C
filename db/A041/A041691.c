/*
 * A041691  Denominators of continued fraction convergents to sqrt(365).
 * https://oeis.org/A041691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 6916*a(n-5) + a(n-10) for n>9; initial values:
 *            a(0)=1,a(1)=9,a(2)=10,a(3)=19,a(4)=181,a(5)=6897,
 *            a(6)=62254,a(7)=69151,a(8)=131405,a(9)=1251796
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/*  a(0) is the first term of the sequence.
    The function returns the nth term (n >= 0).  */
seq_t a(idx_t n)
{
    /* Static cache for computed values. */
    static seq_t *cache    = NULL;
    static size_t  cap     = 0;
    static size_t  max_computed = 0;   /* largest index computed so far */

    /* Initial values for n = 0 .. 9. */
    static const seq_t init_vals[10] = {
        1,      /* a(0) */
        9,      /* a(1) */
        10,     /* a(2) */
        19,     /* a(3) */
        181,    /* a(4) */
        6897,   /* a(5) */
        62254,  /* a(6) */
        69151,  /* a(7) */
        131405, /* a(8) */
        1251796 /* a(9) */
    };

    /* Allocate initial cache for the first 10 terms. */
    if (cache == NULL) {
        cache = (seq_t*)malloc(10 * sizeof(seq_t));
        for (size_t i = 0; i < 10; ++i) {
            cache[i] = init_vals[i];
        }
        cap = 10;
        max_computed = 9;
    }

    /* If n is beyond the range already computed, extend cache and compute. */
    if ((size_t)n > max_computed) {
        /* Extend storage if necessary. */
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed – aborting. */
                abort();
            }
            cache = tmp;
            cap   = new_cap;
        }

        /* Compute the recurrence for all indices up to n. */
        for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
            cache[i] = 6916 * cache[i - 5] + cache[i - 10];
        }
        max_computed = (size_t)n;
    }

    return cache[n];
}
