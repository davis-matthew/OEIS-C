/*
 * A042418  Numerators of continued fraction convergents to sqrt(737).
 * https://oeis.org/A042418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=27, a(n)=a_k*a(n‑1)+a(n‑2) where a_k are the partial
 *            quotients of the continued fraction of sqrt(737)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state across calls */
    static seq_t *p = NULL;
    static size_t cap = 0;
    static idx_t next_idx = 0;   /* next index that needs to be computed */

    /* continued‑fraction parameters */
    static uint64_t m = 0;            /* current m */
    static uint64_t d = 1;            /* current d */
    static uint64_t a_k = 27;         /* current partial quotient a_k */

    /* previous two numerators of convergents: p_{-2}=0, p_{-1}=1 */
    static seq_t prev2 = 0;
    static seq_t prev1 = 1;

    /* ----------  ensure array capacity  ---------- */
    size_t needed = (size_t)n + 1;
    if (p == NULL) {
        cap = needed > 16 ? needed : 16;
        p = (seq_t *)malloc(cap * sizeof(seq_t));
    } else if (needed > cap) {
        size_t new_cap = cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(p, new_cap * sizeof(seq_t));
        p = tmp;
        cap = new_cap;
    }

    /* ----------  compute new terms  ---------- */
    while (next_idx <= n) {
        seq_t cur = (seq_t)a_k * prev1 + prev2;
        p[next_idx] = cur;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;

        /* update continued‑fraction state for next a_k */
        uint64_t m_next = d * a_k - m;
        uint64_t d_next = (737 - m_next * m_next) / d;
        uint64_t a_next = (27 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_k = a_next;

        ++next_idx;
    }

    return p[n];
}
