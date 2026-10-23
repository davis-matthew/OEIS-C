/*
 * A041451  Denominators of continued fraction convergents to sqrt(241).
 * https://oeis.org/A041451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041451: Denominators of continued‑fraction convergents to sqrt(241)
 *
 * Formula: a(n) = 142022136*a(n-17) + a(n-34)   (n > 33)
 * Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=19, a(4)=21, a(5)=124,
 *                 a(6)=393, a(7)=1303, a(8)=1696, a(9)=2999, a(10)=10693,
 *                 a(11)=35078, a(12)=186083, a(13)=221161, a(14)=2176532,
 *                 a(15)=2397693, a(16)=4574225
 *
 * The function below computes the sequence by generating the convergents
 * of sqrt(241) directly; this works for arbitrary sequence element type
 * seq_t.  Results are cached so that any call order is efficient.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache and state */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated capacity (bytes of seq_t) */
    static idx_t len   = 0;   /* number of cached terms (next free index) */

    /* state for continued‑fraction generation */
    static uint64_t m      = 0;                 /* m_k  */
    static uint64_t d      = 1;                 /* d_k  */
    static uint32_t a_cur  = 15;                /* a_k  (a_0 = floor(sqrt(241))) */
    static seq_t  prev_prev = 1;                /* q_{k-2}, initial 1 */
    static seq_t  prev      = 0;                /* q_{k-1}, initial 0 */

    const seq_t sqrt241_floor = (seq_t)15;      /* a_0 */

    /* Safeguard: only enlarge cache when needed */
    if (n >= cap) {
        size_t new_cap = cap ? cap * 2U : 16U;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1U;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms up to n if not already cached */
    for (idx_t k = len; k <= n; ++k) {
        /* q_k = a_k * q_{k-1} + q_{k-2} */
        seq_t qk = (seq_t)a_cur * prev + prev_prev;
        cache[k] = qk;

        /* Shift for next iteration */
        prev_prev = prev;
        prev      = qk;

        /* Update continued‑fraction parameters for the next a_k+1 */
        uint64_t m_next = d * a_cur - m;
        uint64_t d_next = (241ULL - m_next * m_next) / d;
        a_cur = (uint32_t)((sqrt241_floor + m_next) / d_next);
        m     = m_next;
        d     = d_next;
    }

    len = n + 1;   /* cache now contains indices 0 .. n */
    return cache[n];
}
