/*
 * A231184  Coefficients of the nonnegative powers of rho(11) = 2*cos(Pi/11)
 *          when written in the power basis of the degree 5 number field
 *          Q(rho(11)). Negative of the coefficients of the second power.
 * https://oeis.org/A231184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   recurrence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A231184
 *  recurrence: a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 3*a(n-4) + a(n-5)
 *  initial values: a(0) = -1, a(1)=0, a(2)=0, a(3)=3, a(4)=6
 *
 *  The implementation keeps a static cache for n < 10005 and a
 *  dynamically grown cache for larger indices.  All operations are
 *  performed using the type seq_t supplied by the build system.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/recurrence.h"

#define STATIC_MAX 10005   /* safe static range */

/* static cache for small n */
static seq_t static_cache[STATIC_MAX];
static bool  static_inited = false;
static idx_t static_last   = 4; /* highest index computed in static_cache */

/* dynamic cache for larger n */
static seq_t *dyn_cache   = NULL;
static size_t dyn_cap     = 0;  /* allocated size of dyn_cache        */
static idx_t  dyn_last    = 4;  /* largest index computed in dyn_cache*/

/* initialise static cache with the first 5 values */
static void init_static(void)
{
    if (!static_inited) {
        static_cache[0] = (seq_t)(-1);
        static_cache[1] = (seq_t)0;
        static_cache[2] = (seq_t)0;
        static_cache[3] = (seq_t)3;
        static_cache[4] = (seq_t)6;
        static_inited  = true;
    }
}

/* initialise dynamic cache with the first 5 values */
static void init_dynamic(void)
{
    if (!dyn_cache) {
        dyn_cap = 8;                  /* start with a small capacity */
        dyn_cache = (seq_t *)malloc(dyn_cap * sizeof(seq_t));
        if (!dyn_cache) return;       /* memory failure: caller will crash */

        dyn_cache[0] = (seq_t)(-1);
        dyn_cache[1] = (seq_t)0;
        dyn_cache[2] = (seq_t)0;
        dyn_cache[3] = (seq_t)3;
        dyn_cache[4] = (seq_t)6;
        dyn_last    = 4;
    }
}

/* ensure dyn_cache has space for indices up to n */
static void grow_dynamic(idx_t n)
{
    if (n < dyn_cap) return;
    size_t new_cap = dyn_cap ? dyn_cap << 1 : 1;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(dyn_cache, new_cap * sizeof(seq_t));
    if (!tmp) return;   /* on failure we keep old pointer – caller will crash */
    dyn_cache = tmp;
    dyn_cap   = new_cap;
}

/* required function */
seq_t a(idx_t n)
{
    if (n < STATIC_MAX) {           /* use static cache */
        init_static();

        if (n > static_last) {
            for (idx_t i = static_last + 1; i <= n; ++i) {
                static_cache[i] = recurrence(i, &static_cache[i-5]);
            }
            static_last = n;
        }
        return static_cache[n];
    }

    /* use dynamic cache */
    init_dynamic();

    if (n > dyn_last) {
        grow_dynamic(n);
        for (idx_t i = dyn_last + 1; i <= n; ++i) {
            dyn_cache[i] = recurrence(i, &dyn_cache[i-5]);
        }
        dyn_last = n;
    }
    return dyn_cache[n];
}
