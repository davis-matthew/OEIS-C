/*
 * A127614  a(n) = numerator of the continued fraction which has the positive
 *          integers which are <= n and are coprime to n as its terms.
 * https://oeis.org/A127614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of the continued fraction formed by the
 *            positive integers ≤ n that are coprime to n (in increasing
 *            order)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline idx_t gcd_idx_t(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard anyway */

    /* Allocate array big enough to hold all totatives (worst-case n). */
    size_t cap = (size_t)n;
    seq_t *arr = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!arr) return (seq_t)0;     /* allocation failed */

    /* Collect coprime numbers ≤ n in ascending order. */
    size_t k = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (gcd_idx_t(i, n) == 1) {
            arr[k++] = (seq_t)i;
        }
    }

    /* Compute numerator of the continued fraction via backward recurrence. */
    seq_t num = arr[k - 1];
    seq_t den = (seq_t)1;
    for (int j = (int)k - 2; j >= 0; --j) {
        seq_t a_i = arr[j];
        seq_t new_num = a_i * num + den;
        seq_t new_den = num;
        num = new_num;
        den = new_den;
    }

    free(arr);
    return num;
}
