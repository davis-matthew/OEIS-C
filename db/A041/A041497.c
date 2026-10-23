/*
 * A041497  Denominators of continued fraction convergents to sqrt(265).
 * https://oeis.org/A041497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A041497: denominators of convergents to sqrt(265).
    a(n) = 12144*a(n-9) + a(n-18)   for n > 17
    a(0) = 1, a(1) = 3, a(2) = 4, a(3) = 7, a(4) = 18, a(5) = 43,
    a(6) = 61, a(7) = 104, a(8) = 373, a(9) = 12040, …            */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;         /* allocated capacity */
    static idx_t max_n = -1;       /* highest index computed (idx_t == uint64_t) */

    /* initialise cache if first call */
    if (cache == NULL) {
        /* start with an initial capacity large enough for the first 18 terms */
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – unlikely for tests */

        /* --- compute the first 18 terms from the continued fraction of sqrt(265) --- */
        const idx_t D = 265;
        const idx_t a0 = 16;   /* floor(sqrt(265)) */

        /* auxiliary variables for the continued‑fraction algorithm */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;          /* current partial quotient */

        /* convergents: q_{-1} = 0, q_0 = 1 */
        cache[0] = 1;          /* a(0) = q_0 */
        seq_t q_kminus2 = 0;   /* q_{-1} */
        seq_t q_kminus1 = 1;   /* q_0  */

        for (idx_t k = 1; k <= 17; ++k) {
            /* next partial quotient of sqrt(D) */
            idx_t m_next = d * a - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            /* update state */
            m = m_next;
            d = d_next;
            a = a_next;

            /* compute q_k = a_k * q_{k-1} + q_{k-2} */
            seq_t q_k = (seq_t)a * q_kminus1 + q_kminus2;
            cache[k] = q_k;

            /* shift for next step */
            q_kminus2 = q_kminus1;
            q_kminus1 = q_k;
        }
        max_n = 17;
    } else if ((size_t)n + 1 > cap) {
        /* extend cache capacity to accommodate n */
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – unlikely in tests */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n if required */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i < 18) {
                /* i < 18 already filled during initialisation */
                continue;
            }
            /* recurrence: a(i) = 12144*a(i-9) + a(i-18) */
            cache[i] = (seq_t)12144 * cache[i - 9] + cache[i - 18];
        }
        max_n = n;
    }

    return cache[(size_t)n];
}
