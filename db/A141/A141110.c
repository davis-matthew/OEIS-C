/*
 * A141110  Number of cycles and fixed points in the permutation (n, n-2, n-4,
 *          ..., 1, ..., n-3, n-1).
 * https://oeis.org/A141110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of cycles (including fixed points) in the
 *            permutation obtained by reversing prefixes of lengths 2,3,...,n
 *            applied successively to (1,2,...,n)
 * Range:     u8 n <= 4094, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* out of range */
    /* visited[0] is unused, indices run from 1..n */
    unsigned char *visited = (unsigned char *)malloc((size_t)(n + 1));
    if (!visited) return 0;                /* unlikely but safe */
    for (size_t i = 0; i <= (size_t)n; ++i) visited[i] = 0;

    seq_t cycles = 0;
    idx_t half = n / 2;                     /* floor(n/2) */

    for (idx_t i = 1; i <= n; ++i) {
        if (visited[(size_t)i]) continue;

        idx_t j = i;
        do {
            visited[(size_t)j] = 1;
            idx_t p;
            if ((n & 1) == 0) {             /* n even */
                if (j <= half)
                    p = 2 * (half - j + 1);
                else
                    p = 2 * (j - half) - 1;
            } else {                       /* n odd */
                if (j <= half + 1)
                    p = n - 2 * (j - 1);
                else
                    p = 2 * (j - (half + 1));
            }
            j = p;
        } while (!visited[(size_t)j]);

        ++cycles;
    }

    free(visited);
    return cycles;
}
