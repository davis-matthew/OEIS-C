/*
 * A144750  A098777 mod 9.
 * https://oeis.org/A144750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-5) - a(n-6) + a(n-8) for n > 7
 *            Initial values: a(0)=1, a(1)=8, a(2)=7, a(3)=2, a(4)=7, a(5)=5,
 *            a(6)=4, a(7)=5
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle base cases directly */
    if (n < 8) {
        static const seq_t init[8] = {1, 8, 7, 2, 7, 5, 4, 5};
        return init[n];
    }

    /* Allocate an array large enough to hold 0 .. n */
    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t*)malloc(len * sizeof(seq_t));
    if (!arr) return 0;  /* out‑of‑memory protection, unlikely in practice */

    /* Initialize the first eight terms */
    arr[0] = 1;  arr[1] = 8;  arr[2] = 7;  arr[3] = 2;
    arr[4] = 7;  arr[5] = 5;  arr[6] = 4;  arr[7] = 5;

    /* Compute subsequent terms by the given recurrence */
    for (idx_t i = 8; i <= n; ++i) {
        arr[i] = arr[i-2] + arr[i-3] - arr[i-5] - arr[i-6] + arr[i-8];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
