/*
 * A007338  Multiplicative encoding of the Eulerian number triangle.
 * https://oeis.org/A007338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} prime(k)^A008292(n,k) for n >= 1, a(0) =
 *            2, where A008292(n,k) = k*A008292(n-1,k) +
 *            (n-k+1)*A008292(n-1,k-1), A008292(1,1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   a007338_isprime, a007338_powm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a007338_isprime.h"
#include "../utils/a007338_powm.h"

seq_t a(idx_t n) {
    uint64_t *row;
    seq_t result = 1;
    uint64_t p = 1;
    idx_t i, k;

    if (n == 0) return 2;

    row = (uint64_t *)calloc((size_t)(n + 1), sizeof(uint64_t));
    /* row[k] holds Eulerian number A(i, k), k = 0..i-1 */
    row[0] = 1;
    for (i = 2; i <= n; i++) {
        for (k = i - 1; k >= 1; k--)
            row[k] = (k + 1) * row[k] + (i - k) * row[k - 1];
        row[0] = 1;
    }

    for (k = 0; k < n; k++) {
        do { p++; } while (!a007338_isprime(p));
        result *= a007338_powm((seq_t)p, row[k]);
    }

    free(row);
    return result;
}
