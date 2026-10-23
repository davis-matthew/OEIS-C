/*
 * A031549  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 51.
 * https://oeis.org/A031549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   ensure_idx_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A031549: brute‑force search for integers k such that
   the continued‑fraction period of sqrt(k) is even and, after deleting
   the last period term, the middle term is 51.  The search proceeds
   incrementally; results are memoised so that arbitrary call order
   remains efficient. */

#include <stdlib.h>
#include "../utils/ensure_idx_cap.h"

#ifndef _BITSET_H
/* Helper: allocate or grow an array of seq_t values */
static void ensure_seq_cap(seq_t **p, size_t *cap, size_t need) {
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 64;
        if (new_cap < need) new_cap = need;
        seq_t *q = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
        *p = q;
        *cap = new_cap;
    }
}

/* Integer floor square root for unsigned 64‑bit values (no floating). */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = 0xFFFFFFFFULL, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else
            high = mid - 1;
    }
    return ans;
}
#endif

seq_t a(idx_t n) {
    /* static memoised sequence */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0, seq_len = 0;
    /* last k examined; start from 1 to avoid zero */
    static idx_t last_k = 1;

    if (n == 0) return 0; /* out of bounds, undefined but harmless */

    /* Extend memoised array until we have at least n terms. */
    if (seq_len < n) {
        ensure_seq_cap(&seq, &seq_cap, n);
        /* Search new values until seq_len == n. */
        while (seq_len < n) {
            ++last_k;
            idx_t sqrt_k = isqrt_u64(last_k);
            if (sqrt_k * sqrt_k == last_k)   /* perfect square -> skip */
                continue;

            /* Continued–fraction expansion of sqrt(last_k). */
            idx_t a0 = sqrt_k;
            idx_t m = 0;
            idx_t d = 1;
            idx_t a = a0;

            idx_t *terms = NULL;
            size_t terms_cap = 0, terms_len = 0;
            size_t L = 0;                 /* period length */

            while (1) {
                /* next triple (m', d', a'). */
                __int128 prod = (__int128)d * a;
                idx_t m_next = (idx_t)(prod - m);

                __int128 diff = (__int128)last_k - (__int128)m_next * m_next;
                idx_t d_next = (idx_t)(diff / d);

                idx_t a_next = (idx_t)((a0 + m_next) / d_next);

                /* store a_next */
                ensure_idx_cap(&terms, &terms_cap, terms_len + 1);
                terms[terms_len++] = a_next;
                ++L;

                /* end of period? (d_next == 1 && a_next == 2*a0) */
                if (d_next == 1 && a_next == 2 * a0)
                    break;

                m = m_next;
                d = d_next;
                a = a_next;
            }

            /* Check the required conditions: even period and middle term 51. */
            if ((L & 1) == 0) {                     /* even L */
                size_t mid = L / 2 - 1;             /* centre after deletion */
                if (terms[mid] == 51) {
                    seq[seq_len++] = (seq_t)last_k;
                }
            }
            free(terms);
        }
    }

    return seq[n - 1];
}
