/*
 * A006987  Binomial coefficients: C(n,k), 2 <= k <= n-2, sorted, duplicates
 *          removed.
 * https://oeis.org/A006987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form: list every C(m,k), 2 <= k <= m/2 (by
 *            symmetry, the same values as 2 <= k <= m-2), up to the bound
 *            C(n+3,2); merge the triangular numbers C(m,2) with the sorted k
 *            >= 3 values, drop duplicates and take the n-th
 * Range:     u8 n <= 29, u16 n <= 485, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 a006987_u128;

seq_t a(idx_t n) {
    a006987_u128 M = (a006987_u128)n + 3;
    a006987_u128 L = M * (M - 1) / 2;   /* a(n) <= C(n+3,2) */
    size_t cap = 256, cnt = 0, i, j;
    a006987_u128 *e = (a006987_u128 *)malloc(cap * sizeof(a006987_u128));
    a006987_u128 m, k, c, v, last, tri;
    idx_t idx;

    /* binomials C(m,k), 3 <= k <= m/2, not exceeding L */
    for (m = 6; m <= M; m++) {
        if (m * (m - 1) * (m - 2) / 6 > L) break;
        c = m * (m - 1) / 2;
        for (k = 3; 2 * k <= m; k++) {
            c = c * (m - k + 1) / k;
            if (c > L) break;
            if (cnt >= cap) {
                cap *= 2;
                e = (a006987_u128 *)realloc(e, cap * sizeof(a006987_u128));
            }
            e[cnt++] = c;
        }
    }

    /* insertion sort */
    for (i = 1; i < cnt; i++) {
        a006987_u128 x = e[i];
        j = i;
        while (j > 0 && e[j - 1] > x) {
            e[j] = e[j - 1];
            j--;
        }
        e[j] = x;
    }

    /* merge with triangular numbers C(m,2), m >= 4 */
    m = 4;
    j = 0;
    idx = 0;
    last = 0;
    v = 0;
    for (;;) {
        tri = m * (m - 1) / 2;
        if (j < cnt && e[j] < tri) {
            v = e[j++];
        } else if (j < cnt && e[j] == tri) {
            v = tri;
            j++;
            m++;
        } else {
            v = tri;
            m++;
        }
        if (v != last) {
            last = v;
            idx++;
            if (idx == n) break;
        }
    }
    free(e);
    return u128_to_seq(v);
}
