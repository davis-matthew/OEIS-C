/*
 * A097482  a(1) = 1, a(2) = 1, a(n) = floor(sqrt(a(n-2)*a(n-1))) + 3 for n >
 *          2.
 * https://oeis.org/A097482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=1; a(n)=floor(sqrt(a(n‑2)*a(n‑1)))+3 for n>2 */
#include <stdlib.h>
#include <string.h>
#include "../utils/floor_sqrt_seq.h"   /* declares seq_t floor_sqrt_seq(seq_t) */

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static idx_t cap = 0;
    static idx_t last = 0;           /* largest n that has been computed */

    if (n < 1)
        return (seq_t)0;              /* defined only for n >= 1 */

    /* Grow the array so that arr[n] is addressable */
    if (cap < n + 1) {
        idx_t newcap = cap ? cap : 4;
        while (newcap < n + 1) newcap <<= 1;
        seq_t *newarr = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (arr) {
            memcpy(newarr, arr, cap * sizeof(seq_t));
            free(arr);
        }
        arr = newarr;
        cap = newcap;
    }

    /* Compute forward until we reach n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            arr[i] = (seq_t)1;
        } else {
            seq_t prod = arr[i - 2] * arr[i - 1];
            seq_t s    = floor_sqrt_seq(prod);
            arr[i] = s + (seq_t)3;
        }
    }
    last = n;
    return arr[n];
}
