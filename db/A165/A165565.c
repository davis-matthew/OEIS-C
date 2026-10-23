/*
 * A165565  a(n) = n - a(a(a(n-3))) with a(0)=a(1)=a(2)=0.
 * https://oeis.org/A165565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - a(a(a(n-3))), a(0) = a(1) = a(2) = 0
 * Range:     u8 n <= 375, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* array of computed values */
    static idx_t  size = 0;     /* number of values stored */

    /* initialisation */
    if (seq == NULL) {
        seq = (seq_t*)malloc(3 * sizeof(seq_t));
        if (seq == NULL) exit(1);
        seq[0] = seq[1] = seq[2] = 0;
        size = 3;
    }

    if (n < size)
        return seq[n];

    /* grow the cache to hold up to n */
    seq = (seq_t*)realloc(seq, (n + 1) * sizeof(seq_t));
    if (seq == NULL) exit(1);

    for (idx_t i = size; i <= n; ++i) {
        /* compute a(i) for i >= 3; i - 3 is always < i and already known */
        seq_t v1 = seq[i - 3];
        idx_t idx1 = (idx_t)v1;          /* v1 <= i-3 < size, safe cast */

        seq_t v2 = seq[idx1];
        idx_t idx2 = (idx_t)v2;

        seq_t v3 = seq[idx2];

        seq[i] = (seq_t)i - v3;
    }

    size = n + 1;
    return seq[n];
}
