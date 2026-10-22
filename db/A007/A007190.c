/*
 * A007190  Multiplicative encoding of Stirling numbers of 2nd kind.
 * https://oeis.org/A007190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} prime(k)^Stirling2(n, k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static bool small_isprime(uint64_t m) {
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

static seq_t seq_pow(seq_t base, seq_t e) {
    seq_t result = 1;
    while (e > 0) {
        if ((e & 1) != 0) result = result * base;
        e >>= 1;
        if (e > 0) base = base * base;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t *row = alloc_zero(n + 1);
    idx_t i, j;
    row[0] = 1;
    for (i = 1; i <= n; i++) {
        for (j = i; j >= 1; j--)
            row[j] = (seq_t)j * row[j] + row[j - 1];
        row[0] = 0;
    }

    seq_t result = 1;
    uint64_t p = 1;
    for (idx_t k = 1; k <= n; k++) {
        p++;
        while (!small_isprime(p)) p++;
        if (row[k] > 0)
            result = result * seq_pow((seq_t)p, row[k]);
    }

    release(row, n + 1);
    return result;
}
