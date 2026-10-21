/*
 * A093652  Let a(1) = 1, a(2) = 2, a(3) = 7, a(4) = 15 and for n >= 5 set
 *          a(n) = (n*b(n) - b(n-2)) / 2, where b(n) = 4*b(n-2) - b(n-4) for n
 *          >= 5 and b(1) = 1, b(2) = 2, b(3) = 5, b(4) = 8.
 * https://oeis.org/A093652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   b(n) = 4*b(n-2) - b(n-4) with b(1)=1, b(2)=2, b(3)=5, b(4)=8
 *            a(n) = (n*b(n) - b(n-2)) / 2 for n ≥ 5 Initial values: a(1)=1,
 *            a(2)=2, a(3)=7, a(4)=15
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n) {
    /* Handle the first four terms directly. */
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 7;
    if (n == 4) return 15;

    /* Allocate array to hold b(1)…b(n). */
    size_t sz = (size_t)n + 1;                     /* indices 0..n */
    seq_t *b = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!b) return 0;                              /* out of memory */

    /* Initialise base values of b. */
    b[1] = 1;
    b[2] = 2;
    b[3] = 5;
    b[4] = 8;

    /* Compute b(i) for i = 5 … n. */
    for (idx_t i = 5; i <= n; ++i) {
        size_t id = (size_t)i;
        b[id] = 4 * b[id - 2] - b[id - 4];
    }

    /* Compute a(n) using the formula. */
    seq_t result = ((seq_t)n * b[(size_t)n] - b[(size_t)(n - 2)]) / 2;

    free(b);
    return result;
}
