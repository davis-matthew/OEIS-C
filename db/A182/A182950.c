/*
 * A182950  Joint-rank array of the numbers (3*i+2)*3^j, where i>=0, j>=0, by
 *          antidiagonals.
 * https://oeis.org/A182950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the rank of the number (3*i+2)*3^j in the set {
 *            (3*i'+2)*3^j' | i',j' ≥ 0 }, where (i,j) are the antidiagonal
 *            coordinates of n: n = 1 + Σ_{s=0}^{k-1} (s+1) + i with 0 ≤ i ≤ k
 *            and j = k - i. The rank is (i+1) + (3*i+2)*(3^j-1)/2 +
 *            Σ_{k=1}^{⌊log₃(3*i+2)⌋} ⌊(⌊(3*i+2)/3^k⌋-2)/3⌋+1
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* step 1: find diagonal k and i (offset) */
    idx_t k = 0;
    idx_t prev_sum = 0;
    while (prev_sum + (k + 1) < n) {
        prev_sum += k + 1;
        ++k;
    }
    idx_t offset = n - 1 - prev_sum;   /* 0‑based offset on this diagonal */
    idx_t i = offset;
    idx_t j = k - offset;

    /* step 2: base and 3^j */
    seq_t base = (seq_t)3 * i + 2;
    seq_t pow3_j = big_pow((seq_t)3, (seq_t)j);

    /* step 3: rank from terms with j' ≤ j */
    seq_t rank = (seq_t)i + 1;
    if (pow3_j > (seq_t)1) {
        /* (pow3_j-1) is even for j≥1, so division is exact */
        rank += base * (pow3_j - 1) / 2;
    }

    /* step 4: contributions from terms with j' > j */
    seq_t rem = base;           /* rem = floor(base / 3^k) after each division */
    while (true) {
        rem /= 3;
        if (rem < (seq_t)2) break;
        seq_t cnt = (rem - 2) / 3 + 1;
        rank += cnt;
    }

    return rank;
}
