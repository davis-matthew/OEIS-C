/*
 * A007622  Consider Leibniz's harmonic triangle (A003506) and look at the
 *          non-boundary terms. Sequence gives numbers appearing in
 *          denominators, sorted.
 * https://oeis.org/A007622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: the non-boundary denominators of Leibniz's harmonic
 *            triangle are (r+1)*binomial(r,k) for r >= 2 and 1 <= k <= r-1.
 *            Generate every value up to a bound L, sort, remove duplicates,
 *            and double L until there are at least n values; a(n) is the n-th
 *            smallest
 * Range:     u8 n <= 19, u16 n <= 339, u32 n <= 1217, u64 n <= 1217,
 *            u128 n <= 1217, big any n
 * Helpers:   lh_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/lh_cmp.h"

static uint64_t *lh_vals = NULL;
static idx_t lh_cnt = 0;

static void lh_build(uint64_t L) {
    idx_t cap = 1024, cnt = 0;
    uint64_t *arr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (uint64_t r = 2; r * (r + 1) <= L; r++) {
        uint64_t c = 1; /* binomial(r, k) */
        for (uint64_t k = 1; 2 * k <= r; k++) {
            c = c * (r - k + 1) / k;
            uint64_t v = (r + 1) * c;
            if (v > L) break;
            if (cnt >= cap) {
                cap *= 2;
                arr = (uint64_t *)realloc(arr, cap * sizeof(uint64_t));
            }
            arr[cnt++] = v;
        }
    }
    qsort(arr, cnt, sizeof(uint64_t), lh_cmp);
    idx_t w = 0;
    for (idx_t i = 0; i < cnt; i++)
        if (w == 0 || arr[i] != arr[w - 1]) arr[w++] = arr[i];
    free(lh_vals);
    lh_vals = arr;
    lh_cnt = w;
}

seq_t a(idx_t n) {
    if (n > lh_cnt) {
        uint64_t L = 64;
        while (lh_build(L), lh_cnt < n) L *= 2;
    }
    return (seq_t)lh_vals[n - 1];
}
