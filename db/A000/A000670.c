/*
 * A000670  Fubini numbers: number of preferential arrangements of n labeled
 *          elements; or number of weak orders on n labeled elements; or
 *          number of ordered partitions of [n].
 * https://oeis.org/A000670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, nice, easy, changed
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k)*a(n-k), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/grow_arr.h"

seq_t a(idx_t n) {
    static seq_t *A = NULL;   /* memoized terms a(0..len-1) */
    static seq_t *C = NULL;   /* Pascal row binomial(len-1, k) */
    static idx_t len = 0;
    static idx_t cap = 0;

    if (n >= cap) {
        idx_t newcap = cap ? cap : 64;
        idx_t j;
        while (newcap <= n) newcap *= 2;
        A = grow_arr(A, cap, newcap);
        C = grow_arr(C, cap, newcap);
        for (j = cap; j < newcap; j++) {
            A[j] = 0;
            C[j] = 0;
        }
        cap = newcap;
    }

    if (len == 0) {
        A[0] = 1;
        C[0] = 1;
        len = 1;
    }

    while (len <= n) {
        idx_t i = len;
        idx_t k;
        seq_t s = 0;
        /* advance Pascal row from i-1 to i */
        C[i] = 0;
        for (k = i; k >= 1; k--) C[k] += C[k - 1];
        for (k = 1; k <= i; k++) s += C[k] * A[i - k];
        A[i] = s;
        len++;
    }

    return A[n];
}
