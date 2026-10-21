/*
 * A053342  Continued fraction for sqrt(1729).
 * https://oeis.org/A053342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(1729).  a(0)=⌊√1729⌋=41, and for
   k≥0:
     m0=0, d0=1, a0=41
     m_{k+1}=d_k*a_k−m_k
     d_{k+1}=(D−m_{k+1}²)/d_k
     a_{k+1}=⌊(a0+m_{k+1})/d_{k+1}⌋
   with D=1729.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define D 1729U        /* Non‑square integer */
#define A0 41U         /* floor(sqrt(D)) */

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static idx_t    cached_max = (idx_t)-1;   /* no terms computed yet */
    static uint64_t m      = 0;              /* current m_k (starts at 0) */
    static uint64_t d      = 1;              /* current d_k (starts at 1) */
    static uint64_t a_val  = A0;              /* current a_k */

    /* ensure cache large enough */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (cap == 0) ? 1 : cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* memory allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* initialise on first use */
    if (cached_max == (idx_t)-1) {
        cache[0] = (seq_t)A0;
        cached_max = 0;
        m = 0;
        d = 1;
        a_val = A0;
    }

    /* if requested term already computed, return it */
    if (n <= cached_max)
        return cache[n];

    /* compute terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        uint64_t m_next = d * a_val - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (A0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_val = a_next;

        cache[i] = (seq_t)a_val;
    }

    cached_max = n;
    return cache[n];
}
