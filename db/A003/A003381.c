/*
 * A003381  Numbers that are the sum of 3 nonzero 8th powers.
 * https://oeis.org/A003381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all x^8+y^8+z^8 with 1<=x<=y<=z<=L, sort and
 *            deduplicate, keep the values below (L+1)^8 (these are complete),
 *            and grow L until at least n terms are known
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 661, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   s3381_pw8, s3381_sort
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/s3381_pw8.h"
#include "../utils/s3381_sort.h"

static seq_t *s3381_vals = NULL;
static idx_t s3381_cnt = 0;
static bool s3381_all = false;

seq_t a(idx_t n) {
    idx_t L = 8;
    if (n <= s3381_cnt) return s3381_vals[n - 1];
    if (s3381_all) return 0;
    while (1) {
        seq_t *pw = (seq_t *)calloc((size_t)(L + 2), sizeof(seq_t));
        idx_t m = 0, i, j, k, cap, cnt = 0, u;
        bool ovf, limited;
        seq_t bound = 0;
        seq_t *arr;
        for (i = 1; i <= L; i++) {
            pw[i] = s3381_pw8(i, &ovf);
            if (ovf) break;
            m = i;
        }
        limited = false;
        if (m == L) {
            bound = s3381_pw8(L + 1, &ovf);
            if (!ovf) limited = true;
        }
        cap = m * (m + 1) * (m + 2) / 6 + 1;
        arr = (seq_t *)calloc((size_t)cap, sizeof(seq_t));
        for (i = 1; i <= m; i++) {
            for (j = i; j <= m; j++) {
                seq_t s2 = pw[i] + pw[j];
                if (s2 < pw[i]) break;
                if (limited && s2 >= bound) break;
                for (k = j; k <= m; k++) {
                    seq_t s3 = s2 + pw[k];
                    if (s3 < s2) break;
                    if (limited && s3 >= bound) break;
                    arr[cnt++] = s3;
                }
            }
        }
        s3381_sort(arr, cnt);
        u = 0;
        for (i = 0; i < cnt; i++) {
            if (u == 0 || arr[i] != arr[u - 1]) arr[u++] = arr[i];
        }
        free(pw);
        if (s3381_vals) free(s3381_vals);
        s3381_vals = arr;
        s3381_cnt = u;
        if (!limited) s3381_all = true;
        if (n <= s3381_cnt) return s3381_vals[n - 1];
        if (!limited) return 0;
        L = L + L / 2 + 4;
    }
}
