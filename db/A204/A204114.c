/*
 * A204114  Symmetric matrix based on f(i,j) = gcd(L(i), L(j)), where
 *          L=A000032 (Lucas numbers), by antidiagonals.
 * https://oeis.org/A204114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry on the antidiagonal of the matrix M, where
 *            M(i,j) = gcd(L_i, L_j) and L_1 = 1, L_2 = 3, L_k = L_{k-1} +
 *            L_{k-2} (k ≥ 3)
 * Range:     u8 n <= 79, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n = 0 */

    /* Find diagonal d (0‑based) such that (d+1)(d+2)/2 ≥ n. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 < n) ++d;

    /* Index of the first element of this diagonal in the sequence. */
    idx_t sumPrev = d * (d + 1) / 2;

    /* Position k (0‑based) inside the diagonal. */
    idx_t k = n - sumPrev - 1;

    /* Convert to 1‑based matrix indices. */
    idx_t i = 1 + k;
    idx_t j = d + 1 - k;

    /* Largest index needed for Lucas numbers. */
    idx_t need = i > j ? i : j;

    /* Compute the required Lucas numbers on the fly. */
    seq_t Li_val = 0, Lj_val = 0;
    const seq_t L1 = (seq_t)1;
    const seq_t L2 = (seq_t)3;

    if (need == 1) {
        /* Only L1 is needed */
        if (i == 1) Li_val = L1;
        if (j == 1) Lj_val = L1;
    } else {
        /* Handle k = 1 and k = 2 explicitly. */
        if (i == 1) Li_val = L1;
        if (j == 1) Lj_val = L1;
        if (i == 2) Li_val = L2;
        if (j == 2) Lj_val = L2;

        seq_t prev = L1;
        seq_t curr = L2;
        for (idx_t t = 3; t <= need; ++t) {
            seq_t next = prev + curr;
            prev = curr;
            curr = next;
            if (i == t) Li_val = curr;
            if (j == t) Lj_val = curr;
        }
    }

    return gcd(Li_val, Lj_val);
}
