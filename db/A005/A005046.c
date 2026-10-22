/*
 * A005046  Number of partitions of a 2n-set into even blocks.
 * https://oeis.org/A005046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..n} binomial(2*n-1, 2*k-1) * a(n-k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *cache = NULL;   /* cache[i] = a(i) for i < done */
static seq_t *prow = NULL;    /* current Pascal row, length 2*cap */
static idx_t cap = 0;
static idx_t done = 0;
static idx_t prow_r = 0;      /* index of the row currently held in prow */

seq_t a(idx_t n)
{
    if (n >= cap) {
        idx_t nc = cap * 2;
        if (nc <= n) nc = n + 1;
        if (nc < 16) nc = 16;
        if (cache == NULL) {
            cache = alloc_zero(nc);
            prow = alloc_zero(2 * nc);
        } else {
            cache = grow_zero(cache, cap, nc);
            prow = grow_zero(prow, 2 * cap, 2 * nc);
        }
        cap = nc;
    }
    if (done == 0) {
        cache[0] = 1;
        prow[0] = 1;
        prow_r = 0;
        done = 1;
    }
    while (done <= n) {
        idx_t m = done;
        idx_t target = 2 * m - 1;
        /* advance Pascal row to row 'target' using additions only */
        while (prow_r < target) {
            idx_t r = prow_r + 1;
            prow[r] = 1;
            for (idx_t i = r - 1; i >= 1; i--)
                prow[i] += prow[i - 1];
            prow_r = r;
        }
        seq_t s = 0;
        for (idx_t k = 1; k <= m; k++)
            s += prow[2 * k - 1] * cache[m - k];
        cache[m] = s;
        done++;
    }
    return cache[n];
}
