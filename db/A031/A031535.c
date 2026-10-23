/*
 * A031535  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 37.
 * https://oeis.org/A031535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that the continued fraction of sqrt(k) has an even
 *            period and, after deleting the last term of the periodic part,
 *            the central term equals 37. Sequence is computed by exhaustive
 *            search and memoised
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static bool has_property(seq_t k)
{
    seq_t a0 = floor_sqrt(k);
    if (a0 * a0 == k)          /* skip perfect squares */
        return false;

    seq_t m = 0, d = 1, a = a0;

    /* dynamic array for the period terms */
    seq_t *period = NULL;
    size_t pcap = 0, plen = 0;

    for (;;) {
        seq_t m_next = d * a - m;
        seq_t d_next = (k - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        if (plen >= pcap) {                 /* grow buffer */
            size_t newcap = (pcap == 0 ? 8 : pcap * 2);
#ifdef __cplusplus
            seq_t *tmp = new seq_t[newcap];
            for (size_t i = 0; i < plen; i++) tmp[i] = period[i];
            delete [] period;
#else
            seq_t *tmp = (seq_t *)realloc(period, newcap * sizeof(seq_t));
#endif
            period = tmp;
            pcap = newcap;
        }
        period[plen++] = a_next;

        if (a_next == 2 * a0)   /* period finished */
            break;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    if (plen & 1) {                 /* period not even */
#ifdef __cplusplus
        delete [] period;
#else
        free(period);
#endif
        return false;
    }

    /* central term after deleting last term: (len/2)-1 (0‑based) */
    size_t central = (plen >> 1) - 1;
    bool ok = period[central] == 37;

#ifdef __cplusplus
    delete [] period;
#else
    free(period);
#endif
    return ok;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for offset 0 */

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t used = 0;
    static idx_t last_checked = 1;

    if (used >= n)
        return cache[n - 1];

    for (idx_t k = last_checked + 1;; ++k) {
        if (has_property((seq_t)k)) {
            if (used >= cap) {                /* grow cache */
                size_t newcap = (cap == 0 ? 8 : cap * 2);
#ifdef __cplusplus
                seq_t *tmp = new seq_t[newcap];
                for (size_t i = 0; i < used; i++) tmp[i] = cache[i];
                delete [] cache;
#else
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
#endif
                cache = tmp;
                cap = newcap;
            }
            cache[used++] = (seq_t)k;
            if (used >= n) {
                last_checked = k;
                return cache[n - 1];
            }
        }
    }
    /* unreachable */
    return 0;
}
