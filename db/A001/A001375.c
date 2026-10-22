/*
 * A001375  Relational systems on n nodes.
 * https://oeis.org/A001375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1*s_1+2*s_2+...=n} fix
 *            A[s_1,s_2,...]/(1^s_1*s_1!*2^s_2*s_2!*...), fix A[s_1,s_2,...] =
 *            8^Sum_{i,j>=1} gcd(i,j)*s_i*s_j. Computed as (1/n!) * Sum over
 *            cycle types of (class size) * 8^exponent
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   r1375_gcd, r1375_pow8, r1375_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/r1375_gcd.h"
#include "../utils/r1375_pow8.h"
#include "../utils/r1375_rec.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    int N = (int)n;
    int *parts = (int *)malloc((size_t)(N + 1) * sizeof(int));
    int *cnts = (int *)malloc((size_t)(N + 1) * sizeof(int));
    seq_t nfact = (seq_t)1;
    for (int i = 2; i <= N; i++) nfact *= (seq_t)i;
    seq_t sum = (seq_t)0;
    r1375_rec(N, N, parts, cnts, 0, nfact, &sum);
    free(parts);
    free(cnts);
    return sum / nfact;
}
