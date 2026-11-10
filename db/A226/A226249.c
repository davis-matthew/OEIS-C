/*
 * A226249  Positions of nonpositive numbers in the ordering of the rational
 *          numbers at A226247.
 * https://oeis.org/A226249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A005374(n) + n, where A005374(0)=0, A005374(n) = n -
 *            A005374(A005374(A005374(n-1)))
 * Range:     u8 n <= 151, u16 n <= 350, u32 n <= 350, u64 n <= 350,
 *            u128 n <= 350, big any n
 * Validated: standard, pass, 80 terms through n = 339 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A226249_h(idx_t n) {
    long long N = (long long)n;
    seq_t *h = mem_new(seq_t, (idx_t)(N + 1));
    if (h == NULL) return (seq_t)0;
    h[0] = (seq_t)0;
    for (long long i = 1; i <= N; i++) {
        long long x = (long long)h[i - 1];
        long long y = (long long)h[x];
        long long z = (long long)h[y];
        h[i] = (seq_t)i - (seq_t)z;
    }
    seq_t r = h[N];
    mem_free(h);
    return r;
}

seq_t a(idx_t n) {
    return A226249_h(n) + (seq_t)n;
}
