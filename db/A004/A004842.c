/*
 * A004842  Numbers that are the sum of at most 2 positive 5th powers.
 * https://oeis.org/A004842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search - merge the sums i^5 + j^5 (0 <= i <= j) in increasing
 *            order, skipping duplicates
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 3470, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   p5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/p5.h"

seq_t a(idx_t n) {
    idx_t cap = 64, m = 0;
    idx_t *cur = (idx_t *)malloc(cap * sizeof(idx_t));
    idx_t count = 0;
    seq_t last = 0;

    for (;;) {
        bool found = false;
        idx_t bj = 0;
        seq_t best = 0;
        for (idx_t j = 0; j < m; j++) {
            if (cur[j] > j) continue;
            seq_t v = p5(cur[j]) + p5(j);
            if (!found || v < best) {
                found = true;
                best = v;
                bj = j;
            }
        }
        seq_t nv = p5(m);
        if (!found || nv < best) {
            if (m >= cap) {
                cap *= 2;
                cur = (idx_t *)realloc(cur, cap * sizeof(idx_t));
            }
            cur[m] = 0;
            bj = m;
            best = nv;
            m++;
        }
        cur[bj]++;
        if (count == 0 || best != last) {
            count++;
            last = best;
            if (count == n) {
                free(cur);
                return best;
            }
        }
    }
}
