/*
 * A043582  Numbers whose base-3 representation has exactly 2 runs.
 * https://oeis.org/A043582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose base-3 representation has exactly 2 runs
 * Range:     u8 n <= 40, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifndef __cplusplus
#  include <stdlib.h>
#else
#  include <memory>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but guards against accidental 0 */

    /* Find the minimal length L >= 2 such that 2*(L-1)*L >= n */
    idx_t L = 2;
    while ((2 * (L - 1) * L) < n) L++;

    /* Number of terms before length L */
    idx_t prev_count = (L == 2) ? 0 : 2 * (L - 2) * (L - 1);
    idx_t offset = n - prev_count - 1;  /* 0‑based index within length L */

    /* Total terms of length L */
    idx_t count = 4 * (L - 1);

    /* Allocate an array big enough to hold all terms of this length */
    seq_t *arr;
    size_t sz = (size_t)count;
#ifdef __cplusplus
    arr = new seq_t[sz];
#else
    arr = (seq_t *)malloc(sz * sizeof(seq_t));
#endif

    size_t pos = 0;
    for (int d = 1; d <= 2; ++d) {
        for (int e = 0; e <= 2; ++e) {
            if (e == d) continue;
            for (idx_t k = 1; k <= L - 1; ++k) {
                idx_t m = L - k;
                seq_t val = 0;
                for (idx_t i = 0; i < k; ++i) val = val * 3 + d;   /* first run */
                for (idx_t i = 0; i < m; ++i) val = val * 3 + e;   /* second run */
                arr[pos++] = val;
            }
        }
    }

    /* Sort the terms of this length (simple insertion sort, size ≤ 4*(L-1)) */
    for (size_t i = 1; i < sz; ++i) {
        seq_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }

    seq_t result = arr[offset];

#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif
    return result;
}
