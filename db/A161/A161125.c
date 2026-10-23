/*
 * A161125  Number of descents in all involutions of {1,2,...,n}.
 * https://oeis.org/A161125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*A000085(n)/2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 30, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    static seq_t *T = NULL;
    static idx_t size = 0;

    if (size == 0) {
        /* Initialize with T(0)=1, T(1)=1 */
        size = 2;
        T = (seq_t *)malloc(sizeof(seq_t) * size);
        if (!T) return (seq_t)0;
        T[0] = (seq_t)1;
        T[1] = (seq_t)1;
    }

    if (n + 1 > size) {
        seq_t *tmp = (seq_t *)realloc(T, sizeof(seq_t) * (n + 1));
        if (!tmp) return (seq_t)0;
        T = tmp;
        for (idx_t i = size; i <= n; ++i) {
            /* T(i) = T(i-1) + (i-1)*T(i-2) */
            seq_t term = (seq_t)(i - 1) * T[i - 2];
            T[i] = T[i - 1] + term;
        }
        size = n + 1;
    }

    seq_t result = ((seq_t)(n - 1)) * T[n];
    result = result / 2;
    return result;
}
