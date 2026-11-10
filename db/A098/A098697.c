/*
 * A098697  Euler-Seidel matrix T(k,n) with start sequence A000248, read by
 *          antidiagonals.
 * https://oeis.org/A098697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(0,n) = A000248(n), T(k,n) = T(k-1,n) + T(k-1,n+1); read by
 *            antidiagonals, a(n) = T(d-i, i) where n = d(d+1)/2 + i, 0 <= i
 *            <= d
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-07)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

static seq_t A000248_a(idx_t n)
{
    seq_t s = 0;
    seq_t c = 1;
    idx_t k;
    for (k = 0; k <= n; k++) {
        if (k > 0) c = c * (seq_t)(n - k + 1) / (seq_t)k;
        s += c * ipow_idx((seq_t)(n - k), k);
    }
    return s;
}

seq_t a(idx_t n)
{
    idx_t d = 0;
    idx_t i, k, j;
    seq_t *row;
    seq_t result;

    while ((d + 1) * (d + 2) / 2 <= n) d++;
    i = n - d * (d + 1) / 2;
    k = d - i;

    row = mem_new(seq_t, d + 1);
    for (j = 0; j <= d; j++) row[j] = A000248_a(j);
    for (j = 0; j < k; j++) {
        idx_t m;
        for (m = 0; m + j < d; m++) row[m] = row[m] + row[m + 1];
    }
    result = row[i];
    mem_free(row);
    return result;
}
