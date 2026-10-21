/*
 * A200938  Values d for infinite sequence x^3-y^2 = d with increasing
 *          coefficient r=sqrt(x)/|d| or family of solutions Mordell curve
 *          with extension sqrt(2).
 * https://oeis.org/A200938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=648, a(1)=-5400, a(2)=15336, a(3)=-20088, a(4)=100872 *
 *            a(n) = a(n-1) + 6*a(n-2) - 6*a(n-3) - a(n-4) + a(n-5) for n≥5
 * Range:     i8 none, i16 n <= 3, i32 n <= 15, i64 n <= 41, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[5] = {(seq_t)648, (seq_t)-5400, (seq_t)15336,
                                  (seq_t)-20088, (seq_t)100872};

    if (n < 5) return init[n];

    idx_t size = n + 1;
    seq_t *arr;
#ifdef __cplusplus
    arr = new seq_t[size];
#else
    arr = (seq_t*)malloc((size_t)size * sizeof(seq_t));
#endif

    for (idx_t i = 0; i < 5; ++i) arr[i] = init[i];

    for (idx_t i = 5; i <= n; ++i) {
        seq_t val = arr[i-1] + (seq_t)6 * arr[i-2]
                          - (seq_t)6 * arr[i-3]
                          - arr[i-4] + arr[i-5];
        arr[i] = val;
    }

    seq_t result = arr[n];
#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif
    return result;
}
