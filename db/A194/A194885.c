/*
 * A194885  Write n = C(i,4)+C(j,3)+C(k,2)+C(l,1) with i>j>k>l>=0; let L[n] =
 *          [i,j,k,l]; sequence gives list of quadruples L[n], n >= 0.
 * https://oeis.org/A194885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th element of the flattened list of all quadruples
 *            [i, j, k, l] with i > j > k > l ≥ 0 that satisfy n = C(i,4) +
 *            C(j,3) + C(k,2) + C(l,1). The index n is split as n = 4*m + pos
 *            (0 ≤ pos ≤ 3); the value a(n) is the pos‑th component of the
 *            unique quadruple corresponding to m
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   comb1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/comb1.h"

static inline seq_t comb4(idx_t x)
{
    if (x < 4) return 0;
    return ((seq_t)x * (x - 1) * (x - 2) * (x - 3)) / (seq_t)24;
}
static inline seq_t comb3(idx_t x)
{
    if (x < 3) return 0;
    return ((seq_t)x * (x - 1) * (x - 2)) / (seq_t)6;
}
static inline seq_t comb2(idx_t x)
{
    if (x < 2) return 0;
    return ((seq_t)x * (x - 1)) / (seq_t)2;
}

seq_t a(idx_t n)
{
    /* Determine which quadruple (m) the index belongs to */
    idx_t m   = n / 4;
    idx_t pos = n % 4;               /* 0 = i, 1 = j, 2 = k, 3 = l */

    /* Recover i from m */
    idx_t i = 0;
    while (comb4(i) <= (seq_t)m) i++;   /* stop when comb4(i) > m */
    i--;                               /* largest i with comb4(i) ≤ m */

    seq_t rem = (seq_t)m - comb4(i);   /* remaining value after subtracting C(i,4) */

    /* Recover j from the remaining */
    idx_t j = 0;
    while (j < i && comb3(j) <= rem) j++;
    j--;

    seq_t rem2 = rem - comb3(j);       /* subtract C(j,3) */

    /* Recover k from the remaining */
    idx_t k = 0;
    while (k < j && comb2(k) <= rem2) k++;
    k--;

    seq_t rem3 = rem2 - comb2(k);      /* remaining value after C(k,2) */

    /* l is simply the remaining value */
    idx_t l = (idx_t)rem3;

    /* Return the appropriate component of the quadruple */
    switch (pos) {
    case 0: return (seq_t)i;
    case 1: return (seq_t)j;
    case 2: return (seq_t)k;
    case 3: return (seq_t)l;
    default: return 0;
    }
}
