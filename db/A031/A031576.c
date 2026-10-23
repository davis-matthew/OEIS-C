/*
 * A031576  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 78.
 * https://oeis.org/A031576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A031576: numbers k such that the continued fraction for sqrt(k)
   has an even period and, after deleting the last term of the
   periodic part, the central term equals 78. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

#define MAX_PERIOD 8192          /* safe upper bound for period length */

/* buffer to hold one period; re‑used for each k */
static seq_t period[MAX_PERIOD];

/* Return the nth element of A031576 (n >= 1).  The function lazily
   generates terms up to the requested index and caches them for
   subsequent calls. */
seq_t a(idx_t n)
{
    static seq_t *cache     = NULL; /* array of found k's */
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;
    static idx_t  next_k    = 1;     /* last integer examined */

    /* grow cache if necessary */
    if (cache_len + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out‑of‑memory guard – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        ++next_k;
        seq_t k = (seq_t)next_k;

        /* skip perfect squares */
        seq_t a0 = floor_sqrt(k);
        if (a0 * a0 == k) continue;

        /* compute the continued‑fraction period of sqrt(k) */
        int period_len = 0;
        seq_t m = 0, d = 1, a = a0;

        while (1) {
            /* all intermediate products are performed in 128‑bit
               to avoid overflow when seq_t is 64‑bit. */
            seq_t m_next = (seq_t)(((__int128)d * a) - (__int128)m);
            seq_t d_next = (seq_t)((( (__int128)k - (__int128)m_next * m_next)) / (__int128)d);
            seq_t a_next = (seq_t)((a0 + m_next) / d_next);

            period[period_len++] = a_next;

            if (period_len >= MAX_PERIOD) break;   /* safety guard */
            if (a_next == 2 * a0) break;           /* end of the period */

            m = m_next;
            d = d_next;
            a = a_next;
        }

        /* test the required conditions */
        if ((period_len % 2) == 0) {                /* even period */
            seq_t central = period[period_len / 2 - 1]; /* central term after deletion */
            if (central == (seq_t)78) {
                /* store the valid k */
                cache[cache_len++] = k;
            }
        }
    }

    return cache[n - 1];
}
