/*
 * A106637  Accumulation of permutation sequence on three symbols such that
 *          a(n+2) - 2*a(n+1) - a(n) = 0 overall.
 * https://oeis.org/A106637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n ≥ 11 Initial values:
 *            a(1)=2, a(2)=3, a(3)=5, a(4)=8, a(5)=11, a(6)=13, a(7)=14,
 *            a(8)=15, a(9)=17, a(10)=20
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[10] = {2,3,5,8,11,13,14,15,17,20};

    if (n == 0) return 0;
    if (n <= 10) return init[n - 1];

    /* Allocate array to store terms up to n */
    size_t len = (size_t)(n + 1);
    seq_t *seq = (seq_t *)malloc(len * sizeof(seq_t));
    if (!seq) return 0;  /* allocation failure – unlikely for n <= 10000 */

    /* Initialize first 10 terms */
    for (idx_t i = 1; i <= 10; ++i) seq[i] = init[i - 1];

    /* Compute subsequent terms using the linear recurrence */
    for (idx_t i = 11; i <= n; ++i) {
        seq[i] = seq[i - 1] + seq[i - 9] - seq[i - 10];
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
