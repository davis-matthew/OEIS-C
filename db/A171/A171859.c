/*
 * A171859  a(n) = Bell(n) - Sum_{j=0..n-1} Bell(j), where the Bell numbers
 *          are given in A000110.
 * https://oeis.org/A171859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n) - Sum_{j=0..n-1} Bell(j)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A171859: a(n) = Bell(n) - Sum_{j<n} Bell(j). Bell numbers from Aitken's
 * triangle (each row starts with the last entry of the one before, and each
 * entry adds its left neighbour and the entry above it), ring operations only,
 * so exact modulo the width. */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    const size_t N = (size_t)n;
    seq_t *row = mem_new(seq_t, N + 1), *nxt = mem_new(seq_t, N + 1);
    if (!row || !nxt) abort();
    seq_t sum = (seq_t)0, bell = (seq_t)1;        /* Bell(0) */
    row[0] = (seq_t)1;
    for (size_t m = 1; m <= N; ++m) {
        sum += bell;                               /* Sum_{j<m} Bell(j) */
        nxt[0] = row[m - 1];
        for (size_t j = 1; j <= m; ++j) nxt[j] = nxt[j - 1] + row[j - 1];
        seq_t *t = row; row = nxt; nxt = t;
        bell = row[0];                             /* Bell(m) */
    }
    mem_free(row);
    mem_free(nxt);
    return bell - sum;
}
