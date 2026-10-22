/*
 * A007188  Multiplicative encoding of Pascal triangle: Product p(i+1)^C(n,i).
 * https://oeis.org/A007188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=0..n} prime(i+1)^C(n,i)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   p7188_isprime, p7188_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/p7188_isprime.h"
#include "../utils/p7188_pow.h"

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
        do { p++; } while (!p7188_isprime(p));
        result *= p7188_pow((seq_t)p, row[i]);
    }
    free(row);
    return result;
}
