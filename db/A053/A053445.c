/*
 * A053445  Second differences of partition numbers A000041.
 * https://oeis.org/A053445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = p(n+2) - 2*p(n+1) + p(n), where p(k) is the number of
 *            partitions of k
 * Range:     u8 n <= 29, u16 n <= 67, u32 n <= 176, u64 n <= 517,
 *            u128 n <= 1676, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A053445 (Second differences of partition numbers). */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* We keep a memoised table of partition numbers p[0..max]. */
    static seq_t *p = NULL;         /* array of partition numbers p[i]  */
    static idx_t max_calc = 0;      /* highest index stored (inclusive)  */
    static size_t cap = 0;          /* current allocated size (>=max_calc+1) */

    /* we need values up to n+2 */
    idx_t need = n + 2;
    if (need > max_calc) {
        /* enlarge allocation if necessary */
        if (cap <= need) {
            size_t newcap = cap ? cap * 2 : 16;
            while (newcap <= need) newcap *= 2;
            p = (seq_t*)realloc(p, newcap * sizeof(seq_t));
            cap = newcap;
        }
        /* initialise new entries */
        if (max_calc == 0) {
            p[0] = (seq_t)1;          /* p(0) = 1 */
            max_calc = 0;
        }
        /* compute partition numbers from max_calc+1 up to need */
        for (idx_t m = max_calc + 1; m <= need; ++m) {
            seq_t pos = 0;
            seq_t neg = 0;
            for (idx_t k = 1;; ++k) {
                /* generalized pentagonal number g1 = k*(3k-1)/2 */
                idx_t g1 = k * ((idx_t)3 * k - 1) / 2;
                if (g1 > m) break;
                if (k & 1) pos += p[m - g1];
                else       neg += p[m - g1];

                /* generalized pentagonal number g2 = k*(3k+1)/2 */
                idx_t g2 = k * ((idx_t)3 * k + 1) / 2;
                if (g2 > m) continue;
                if (k & 1) pos += p[m - g2];
                else       neg += p[m - g2];
            }
            p[m] = pos - neg;   /* pos >= neg for partition numbers */
        }
        max_calc = need;
    }

    /* a(n) = p(n+2) - 2*p(n+1) + p(n) */
    seq_t res = p[n + 2] + p[n];
    seq_t twice = p[n + 1];
    twice += twice;
    res -= twice;
    return res;
}
