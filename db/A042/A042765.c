/*
 * A042765  Denominators of continued fraction convergents to sqrt(913).
 * https://oeis.org/A042765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1031468486160814*a(n-32) - a(n-64) (Denominators of
 *            continued‑fraction convergents to sqrt(913)) Initial terms:
 *            a(0)=1, a(1)=4, a(2)=5, a(3)=9, a(4)=14,
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;     /* array capacity */
    static size_t sz  = 0;     /* number of terms stored (cache[0]..cache[sz-1]) */
    static uint64_t m = 0, d = 1, a_cur;
    static seq_t prev2 = 0, prev1 = 1;  /* q_{-1}=0, q_0=1 */
    static const uint64_t D   = 913ULL;
    static const uint64_t a0  = 30ULL;   /* floor(sqrt(913)) */
    static bool initialized = false;

    if (!initialized) {
        /* initialisation */
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(0) */
        sz = 1;
        prev2 = (seq_t)0;     /* q_{-1} */
        prev1 = (seq_t)1;     /* q_0   */
        m = 0;                /* m_0   */
        d = 1;                /* d_0   */
        a_cur = a0;           /* a_0   */
        initialized = true;
    }

    /* generate terms up to index n (if needed) */
    while (sz <= n) {
        /* compute next partial quotient a_{sz} */
        uint64_t m_next = d * a_cur - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* compute next denominator q_{sz} = a_next*q_{sz-1} + q_{sz-2} */
        seq_t q_next = ((seq_t)a_next) * prev1 + prev2;

        /* store it, expanding cache if necessary */
        if (sz >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }
        cache[sz] = q_next;

        /* advance state */
        prev2 = prev1;
        prev1 = q_next;
        m = m_next;
        d = d_next;
        a_cur = a_next;
        ++sz;
    }

    return cache[(size_t)n];
}
