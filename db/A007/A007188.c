/*
 * A007188  Multiplicative encoding of Pascal triangle: Product p(i+1)^C(n,i).
 * https://oeis.org/A007188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=0..n} prime(i+1)^C(n,i)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-03)
 * Helpers:   ipow_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_u64.h"
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    uint64_t *row = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    idx_t i, j;
    row[0] = 1;
    for (i = 1; i <= n; i++) {
        row[i] = 1;
        for (j = i - 1; j >= 1; j--) row[j] += row[j - 1];
    }
    seq_t result = 1;
    uint64_t p = 1;
    for (i = 0; i <= n; i++) {
        do { p++; } while (!is_prime_u64(p));
        result *= ipow_u64((seq_t)p, row[i]);
    }
    free(row);
    return result;
}
