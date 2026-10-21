/*
 * A240880  Expansion of g.f.: (-1 + sqrt(1+12*x+48*x^2)) / (6*x).
 * https://oeis.org/A240880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 13, i64 n <= 25, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+3)*a(n+2)+6*(2*n+3)*a(n+1)+48*n*a(n)=0, a(0)=1, a(1)=1 */
#include <stdlib.h>

static seq_t *cache = NULL;     /* memoised values of the sequence   */
static size_t   cap   = 0;      /* allocated capacity of cache       */
static size_t   len   = 0;      /* number of values currently stored */

/* Ensure the cache can hold at least `size` elements.  */
static void ensure_capacity(size_t size)
{
    if (size <= cap) return;
    size_t new_cap = (cap == 0) ? 4 : cap << 1;
    while (new_cap < size) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cap   = new_cap;
}

/* Return the nth term of the sequence.  */
seq_t a(idx_t n)
{
    /* Initialise cache on first call.  */
    if (len == 0) {
        ensure_capacity(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        len = 2;
    }

    /* If n is already cached, return it immediately.  */
    if ((idx_t)len > n) return cache[n];

    /* Compute successive terms up to n.  */
    for (idx_t i = len; i <= n; ++i) {
        /* Ensure capacity for index i.  */
        size_t need = (size_t)i + 1;
        if (need > cap) ensure_capacity(need);

        if (i < 2) continue;          /* base values already set */

        seq_t val;
        if (sizeof(seq_t) <= 8) {      /* 64‑bit or smaller types */
            __int128 si = (__int128)i;
            __int128 t1 = ((__int128)12 * si - 6) * (__int128)cache[i-1];
            __int128 t2 = ((__int128)48 * si - 96) * (__int128)cache[i-2];
            __int128 num = -(t1 + t2);
            __int128 den = si + 1;
            val = (seq_t)(num / den);
        } else {                       /* arbitrary‑precision types */
            seq_t si = (seq_t)i;
            seq_t term1 = ((seq_t)12 * si - 6) * cache[i-1];
            seq_t term2 = ((seq_t)48 * si - 96) * cache[i-2];
            val = -(term1 + term2) / (si + 1);
        }
        cache[i] = val;
    }

    len = n + 1;
    return cache[n];
}
