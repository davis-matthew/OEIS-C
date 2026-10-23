/*
 * A180289  Number of arrangements of n indistinguishable balls in n boxes
 *          with the maximum number of balls in any box equal to 9.
 * https://oeis.org/A180289
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [z^n](1+z+…+z^9)^n – [z^n](1+z+…+z^8)^n (Number of
 *            compositions of n into n parts, each part ≤9, with at least one
 *            part=9)
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 21, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n < 9) return 0;   /* offset is 9; values below are undefined */

    /* part with bound 9 */
    seq_t pos1 = 0, neg1 = 0;
    idx_t maxj1 = n / 10;
    for (idx_t j = 0; j <= maxj1; ++j) {
        seq_t term = binom(n, j) * binom(2 * n - 10 * j - 1, n - 10 * j);
        if ((j & 1) == 0)
            pos1 += term;
        else
            neg1 += term;
    }
    seq_t sum1 = pos1 - neg1;   /* pos1 >= neg1 for all n */

    /* part with bound 8 */
    seq_t pos2 = 0, neg2 = 0;
    idx_t maxj2 = n / 9;
    for (idx_t j = 0; j <= maxj2; ++j) {
        seq_t term = binom(n, j) * binom(2 * n - 9 * j - 1, n - 9 * j);
        if ((j & 1) == 0)
            pos2 += term;
        else
            neg2 += term;
    }
    seq_t sum2 = pos2 - neg2;

    return sum1 - sum2;   /* final result; guaranteed non‑negative for n≥9 */
}
