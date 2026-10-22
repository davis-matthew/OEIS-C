/*
 * A002871  a(n) = max_{k=0..n} 2^k*A048993(n,k).
 * https://oeis.org/A002871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = max_{k=0..n} 2^k*Stirling2(n,k), with the Stirling2 row
 *            from S(i,k) = k*S(i-1,k) + S(i-1,k-1), S(0,0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   a001028_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001028_grow.h"

static seq_t *a002871_S = NULL;
static idx_t a002871_cap = 0;
static idx_t a002871_row = 0;
static int a002871_init = 0;

seq_t a(idx_t n)
{
    idx_t k;
    if (n + 1 > a002871_cap) {
        idx_t nc = n + 1;
        if (nc < 2 * a002871_cap) nc = 2 * a002871_cap;
        a002871_S = a001028_grow(a002871_S, a002871_cap, nc);
        a002871_cap = nc;
    }
    if (!a002871_init || a002871_row > n) {
        for (k = 0; k < a002871_cap; k++) a002871_S[k] = 0;
        a002871_S[0] = 1;
        a002871_row = 0;
        a002871_init = 1;
    }
    while (a002871_row < n) {
        a002871_row++;
        for (k = a002871_row; k > 0; k--)
            a002871_S[k] = (seq_t)k * a002871_S[k] + a002871_S[k - 1];
        a002871_S[0] = 0;
    }

    seq_t best = 0;
    seq_t p = 1;
    for (k = 0; k <= n; k++) {
        seq_t term = p * a002871_S[k];
        if (term > best) best = term;
        p *= 2;
    }
    return best;
}
