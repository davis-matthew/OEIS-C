/*
 * A217786  Expansion of (psi(x^3) / psi(x))^2 in powers of x where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A217786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 35, i32 n <= 112, i64 n <= 385,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 900 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A217786 – expansion of (psi(x^3)/psi(x))^2
 *  a(0) = 1
 *  For n ≥ 1
 *      a(n) = (1/n) * Σ_{k=1..n} c(k) * a(n−k)
 *  where
 *      c(k) = Σ_{d|k} d * s(d)
 *      s(d) = -2, 2, 0, 2, -2, 0  (period 6, index d mod 6)
 *
 *  The recurrence comes from the Euler transform of the period‑6
 *  sequence s(d).  The coefficients a(n) are computed lazily and
 *  cached so that out‑of‑order calls are handled efficiently.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* return s(d) for d ≥ 1 : period 6 sequence [-2, 2, 0, 2, -2, 0] */
static int s_of_d(idx_t d)
{
    switch (d % 6) {
        case 1:  return -2;
        case 2:  return  2;
        case 3:  return  0;
        case 4:  return  2;
        case 5:  return -2;
        case 0:  return  0;
    }
    /* unreachable */
    return 0;
}

static seq_t *a_cache = NULL;     /* stored a(n) values */
static size_t a_cap = 0;          /* capacity of a_cache */
static long long *c_cache = NULL; /* stored c(k) values   */
static size_t c_cap = 0;          /* capacity of c_cache */

/* extend caches so that indices up to n are available */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);      /* need a[0..n] and c[0..n] */

    if (needed > a_cap) {
        /* grow a_cache */
        size_t new_a_cap = a_cap == 0 ? 1 : a_cap;
        while (new_a_cap < needed) new_a_cap <<= 1;
        a_cache = mem_resize(seq_t, a_cache, new_a_cap);
        if (a_cache == NULL) { abort(); }

        /* grow c_cache */
        size_t new_c_cap = c_cap == 0 ? 1 : c_cap;
        while (new_c_cap < needed) new_c_cap <<= 1;
        c_cache = (long long*)realloc(c_cache, new_c_cap * sizeof(long long));
        if (c_cache == NULL) { abort(); }

        /* initialize new entries of a_cache and c_cache */
        size_t i;
        if (a_cap == 0) {
            a_cache[0] = 1;          /* a(0) = 1 */
            a_cap = 1;
            c_cache[0] = 0;
            c_cap = 1;
        }

        /* compute c(k) for new indices */
        for (i = a_cap; i < needed; ++i) {
            long long sum = 0;
            for (idx_t d = 1; d <= i; ++d) {
                if ((i % d) == 0) {
                    sum += (long long)d * s_of_d(d);
                }
            }
            c_cache[i] = sum;
        }

        /* compute a(n) for new indices */
        for (i = a_cap; i < needed; ++i) {
            __int128 acc = 0;
            for (idx_t k = 1; k <= i; ++k) {
                acc += (__int128)c_cache[k] * ( (__int128)a_cache[i - k] );
            }
            a_cache[i] = (seq_t)(acc / (idx_t)i);
        }

        a_cap = needed;
        c_cap = needed;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[(size_t)n];
}
