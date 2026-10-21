/*
 * A230912  Number of nX5 0..7 arrays x(i,j) with each element horizontally or
 *          antidiagonally next to at least one element with value (x(i,j)+1)
 *          mod 8, and upper left element zero.
 * https://oeis.org/A230912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 50*a(n-1) -580*a(n-2) +9636*a(n-3) -75225*a(n-4)
 *                +427310*a(n-5) -1370674*a(n-6) +496852*a(n-7)
 *                +569224*a(n-8) +39600*a(n-9)   for n > 10
 * Initial values (1‑based):
 *   a(1) = 0
 *   a(2) = 2
 *   a(3) = 164
 *   a(4) = 5714
 *   a(5) = 221768
 *   a(6) = 9144134
 *   a(7) = 372256436
 *   a(8) = 15083831290
 *   a(9) = 611930935216
 *   a(10) = 24834134356510
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined offset */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* allocated entries (indices 0..cache_size-1) */
    static idx_t max_computed = 0;      /* largest n that has been computed */

    /* ensure capacity for indices up to n */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* allocation failure */
        /* old entries are kept, new ones are uninitialized */
        cache = tmp;
        cache_size = new_size;
    }

    /* initial values (1‑based) */
    static const seq_t init[11] = {
        0ULL,                     /* dummy index 0 */
        0ULL,                     /* a(1) */
        2ULL,                     /* a(2) */
        164ULL,                   /* a(3) */
        5714ULL,                  /* a(4) */
        221768ULL,                /* a(5) */
        9144134ULL,               /* a(6) */
        372256436ULL,             /* a(7) */
        15083831290ULL,           /* a(8) */
        611930935216ULL,          /* a(9) */
        24834134356510ULL         /* a(10) */
    };

    /* copy initial terms into cache if not already present */
    if (max_computed < 10) {
        idx_t up = (cache_size > 10) ? 10 : cache_size - 1;
        for (idx_t i = 1; i <= up; ++i)
            cache[i] = init[i];
        max_computed = up;
    }

    /* if requested term already computed, return it immediately */
    if (n <= max_computed) return cache[n];

    /* compute further terms up to n using the 9‑term recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 10) { /* safety guard – copy known value */
            cache[i] = init[i];
            continue;
        }
        /* linear recurrence with integer arithmetic */
        cache[i] =
            cache[i-1] *  (seq_t)50
          - cache[i-2] *  (seq_t)580
          + cache[i-3] *  (seq_t)9636
          - cache[i-4] *  (seq_t)75225
          + cache[i-5] *  (seq_t)427310
          - cache[i-6] *  (seq_t)1370674
          + cache[i-7] *  (seq_t)496852
          + cache[i-8] *  (seq_t)569224
          + cache[i-9] *  (seq_t)39600;
    }

    max_computed = n;
    return cache[n];
}
