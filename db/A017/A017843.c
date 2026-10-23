/*
 * A017843  Expansion of 1/(1-x^5-x^6-x^7-x^8-x^9-x^10-x^11-x^12).
 * https://oeis.org/A017843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ordered compositions of n into parts 5..12
 *            Recurrence: a(0) = 1; a(n)=0 for 1≤n<5; a(n) =
 *            a(n-5)+a(n-6)+…+a(n-12) for n≥5
 * Range:     u8 n <= 28, u16 n <= 49, u32 n <= 90, u64 n <= 174,
 *            u128 n <= 341, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* capacity of cache */
    static size_t used = 0;  /* number of values computed (highest index+1) */

    /* initial allocation and seed base cases */
    if (cap == 0) {
        grow_seq(&cache, &cap, 13);     /* indices 0..12 */
        for (idx_t i = 1; i < 5; ++i)
            cache[i] = 0;
        cache[0] = 1;
        /* compute for 5..12 using recurrence */
        for (idx_t i = 5; i < 13; ++i) {
            seq_t sum = 0;
            for (int j = 5; j <= 12; ++j) {
                if (i >= j)
                    sum += cache[i - j];
            }
            cache[i] = sum;
        }
        used = 13;
    }

    /* enlarge cache if needed and compute missing terms */
    if ((idx_t)cap <= n) {
        grow_seq(&cache, &cap, (size_t)n + 1);
    }
    if (used <= n) {
        for (idx_t i = used; i <= n; ++i) {
            if (i < 5) {
                cache[i] = 0;
            } else {
                seq_t sum = 0;
                for (int j = 5; j <= 12; ++j) {
                    if (i >= (idx_t)j)
                        sum += cache[i - j];
                }
                cache[i] = sum;
            }
        }
        used = (size_t)n + 1;
    }

    return cache[n];
}
