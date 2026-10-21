/*
 * A099568  Expansion of (1-x)/((1-2*x)*(1-x-x^3)).
 * https://oeis.org/A099568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) - 2*a(n-4) Initial values:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=9 This implementation recomputes
 *            the needed terms on each call using a short dynamic array, so it
 *            works for any call order
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the initial values directly for n < 4 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)9;

    /* Allocate an array for indices 0 .. n */
    size_t sz = (size_t)(n + 1);
    seq_t *arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!arr) {
        /* In an OEIS test harness this should never happen; exit on error */
        exit(1);
    }

    /* Set the first four terms */
    arr[0] = (seq_t)1;
    arr[1] = (seq_t)2;
    arr[2] = (seq_t)4;
    arr[3] = (seq_t)9;

    /* Compute the sequence up to a(n) */
    for (idx_t i = 4; i <= n; ++i) {
        arr[i] =  (seq_t)3 * arr[i - 1]
                - (seq_t)2 * arr[i - 2]
                + arr[i - 3]
                - (seq_t)2 * arr[i - 4];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
