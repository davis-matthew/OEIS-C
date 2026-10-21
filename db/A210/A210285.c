/*
 * A210285  Number of 2 X 2 matrices with all elements in {0,1,...,n} and
 *          determinant floor(n/2).
 * https://oeis.org/A210285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=0}^{n²} P(m)·P(m+⌊n/2⌋) where * P(t) is the number
 *            of ordered pairs (x,y) with 0≤x,y≤n and x·y=t
 * Range:     u8 n <= 11, u16 n <= 143, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    const idx_t N = n;
    const idx_t prodMax = N * N;
    const size_t arrSize = (size_t)(prodMax + 1);

    /* Allocate array for product frequencies (uint16_t is enough:
       the maximum count for any product is ≤ n+1 ≤ 10001 < 65535). */
    uint16_t *P = (uint16_t *)calloc(arrSize, sizeof(uint16_t));
    if (!P)
        return 0; /* allocation failure – should not happen in tests */

    /* Count all product pairs (a,d) */
    for (idx_t a = 0; a <= N; ++a) {
        for (idx_t d = 0; d <= N; ++d) {
            uint64_t idx = (uint64_t)a * (uint64_t)d;
            P[idx]++; /* safe, counts fit in uint16_t */
        }
    }

    /* Desired determinant */
    const idx_t k = N / 2;  /* floor(n/2) */

    /* Sum the product of frequencies */
    unsigned __int128 total = 0;
    for (idx_t m = 0; m <= prodMax; ++m) {
        idx_t m2 = m + k;
        if (m2 > prodMax) continue;
        total += (unsigned __int128)P[m] * (unsigned __int128)P[m2];
    }

    free(P);
    return (seq_t)total;
}
