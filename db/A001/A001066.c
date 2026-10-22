/*
 * A001066  Dimensions (sorted, with duplicates removed) of real simple Lie
 *          algebras.
 * https://oeis.org/A001066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n-th smallest distinct value of d or 2d, for d in A003038
 *            = { (k+1)^2-1 (A_k, k>=1), k(2k+1) (B_k, k>=2), k(2k-1) (D_k,
 *            k>=4), 14, 52, 78, 133, 248 }; computed by enumerating up to a
 *            growing bound, sorting and deduplicating
 * Range:     u8 n <= 55, u16 n <= 1036, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001066_cmp, a001066_push
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a001066_cmp.h"
#include "../utils/a001066_push.h"

seq_t a(idx_t n) {
    static const uint64_t excep[5] = {14, 52, 78, 133, 248};
    uint64_t limit = 1024;
    for (;;) {
        uint64_t s = 1;
        while ((s + 1) * (s + 1) <= limit) s++;
        size_t cap = (size_t)(6 * (s + 3) + 20);
        uint64_t *v = (uint64_t *)malloc(cap * sizeof(uint64_t));
        size_t cnt = 0;
        uint64_t k;
        for (k = 1; (k + 1) * (k + 1) - 1 <= limit; k++)
            a001066_push(v, &cnt, (k + 1) * (k + 1) - 1, limit);
        for (k = 2; k * (2 * k + 1) <= limit; k++)
            a001066_push(v, &cnt, k * (2 * k + 1), limit);
        for (k = 4; k * (2 * k - 1) <= limit; k++)
            a001066_push(v, &cnt, k * (2 * k - 1), limit);
        for (int i = 0; i < 5; i++)
            a001066_push(v, &cnt, excep[i], limit);
        qsort(v, cnt, sizeof(uint64_t), a001066_cmp);
        idx_t seen = 0;
        for (size_t i = 0; i < cnt; i++) {
            if (i == 0 || v[i] != v[i - 1]) {
                seen++;
                if (seen == n) {
                    uint64_t r = v[i];
                    free(v);
                    return (seq_t)r;
                }
            }
        }
        free(v);
        limit *= 4;
    }
}
