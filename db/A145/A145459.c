/*
 * A145459  Exponential transform of C(n,9) = A000582.
 * https://oeis.org/A145459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Exponential transform of C(n,9)=A000582 a(n) = ∑_{p=0}^{n-9}
 *            binom(n-1,p) * a(p) * C(n-p,9) a(0)=1, a(1..8)=0
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static size_t cache_sz = 0;
    static const seq_t P9 = (seq_t)362880; /* 9! */

    if (n + 1 > cache_sz) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(arr, new_size * sizeof(seq_t));
        if (!tmp && new_size != 0) abort();
        arr = tmp;

        for (idx_t i = cache_sz; i <= n; ++i) {
            if (i == 0) {
                arr[0] = (seq_t)1;
            } else if (i < 9) {
                arr[i] = (seq_t)0;
            } else {
                seq_t sum = (seq_t)0;
                seq_t binom = (seq_t)1; /* C(i-1,0) */

                for (idx_t p = 0; p <= i - 9; ++p) {
                    seq_t ap = arr[p];
                    if (ap != (seq_t)0) {
                        idx_t m = i - p;
                        seq_t c = (seq_t)m * (m - 1) * (m - 2) * (m - 3) *
                                  (m - 4) * (m - 5) * (m - 6) * (m - 7) * (m - 8);
                        c /= P9; /* C(m,9) */
                        sum += binom * ap * c;
                    }
                    if (p < i - 9) {
                        binom = binom * (seq_t)(i - 1 - p) / (seq_t)(p + 1);
                    }
                }
                arr[i] = sum;
            }
        }
        cache_sz = n + 1;
    }
    return arr[n];
}
