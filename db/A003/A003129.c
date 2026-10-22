/*
 * A003129  Number of transfer impedances of an n-terminal network.
 * https://oeis.org/A003129
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k)*binomial(binomial(k,2),2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t *row = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    idx_t i, k;
    seq_t result = 0;
    if (!row) return 0;

    /* row[k] = Stirling2(i, k), built in place row by row */
    row[0] = 1;
    for (k = 1; k <= n; k++) row[k] = 0;
    for (i = 1; i <= n; i++) {
        for (k = i; k >= 1; k--)
            row[k] = (seq_t)k * row[k] + row[k - 1];
        row[0] = 0;
    }

    for (k = 2; k <= n; k++) {
        /* binomial(binomial(k,2),2) is computed exactly in 64 bits (k is small) */
        uint64_t c = (uint64_t)k * (uint64_t)(k - 1) / 2;
        uint64_t bc = (c >= 2) ? (c % 2 == 0 ? (c / 2) * (c - 1) : c * ((c - 1) / 2)) : 0;
        if (bc == 0) continue;
        result += row[k] * (seq_t)bc;
    }

    free(row);
    return result;
}
