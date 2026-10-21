/*
 * A063730  Number of solutions to w^2 + x^2 + y^2 + z^2 = n in positive
 *          integers.
 * https://oeis.org/A063730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered solutions to w²+x²+y²+z² = n in
 *            positive integers
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t count = 0;

    /* Allocate a flag array to test whether an integer up to n is a perfect square. */
    size_t N = (size_t)n;
    bool *isSq = (bool *)calloc(N + 1, sizeof(bool));
    if (!isSq)
        return count;  /* allocation failure – return 0 as a safe fallback */

    /* Mark all squares k² (k ≥ 1) that are ≤ n. */
    for (seq_t k = 1; k * k <= sn; ++k) {
        seq_t sq = k * k;
        if ((size_t)sq <= N)
            isSq[(size_t)sq] = true;
    }

    /* Iterate over w, x, y and determine whether the remaining sum is a positive square. */
    for (seq_t w = 1; w * w <= sn; ++w) {
        seq_t w2 = w * w;
        seq_t rem1 = sn - w2;
        for (seq_t x = 1; x * x <= rem1; ++x) {
            seq_t x2 = x * x;
            seq_t rem2 = rem1 - x2;
            for (seq_t y = 1; y * y <= rem2; ++y) {
                seq_t y2   = y * y;
                seq_t zsq  = rem2 - y2;          /* remaining sum for z² */
                if (zsq == 0)                   /* z must be ≥ 1 */
                    continue;
                if ((size_t)zsq <= N && isSq[(size_t)zsq])
                    count += 1;
            }
        }
    }

    free(isSq);
    return count;
}
