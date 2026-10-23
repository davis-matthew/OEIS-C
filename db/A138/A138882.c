/*
 * A138882  Triangle read by rows: row n lists divisors of n-th even
 *          superperfect number A061652(n).
 * https://oeis.org/A138882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2^e where e = n - (sum_{k=1}^{i-1} p_k) - 1, and i is the
 *            smallest index with sum_{k=1}^{i} p_k ≥ n, with p_k the k‑th
 *            Mersenne prime exponent
 * Range:     u8 n <= 25, u16 n <= 46, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* Mersenne prime exponents (p) sufficient for n up to ~100000
   (sum of the first 25 exponents ≈ 93000). */
static const idx_t mersenne_exponents[] = {
    2, 3, 5, 7, 13, 17, 19, 31, 61, 89,
    107, 127, 521, 607, 1279, 2203, 2281, 3217,
    4253, 4423, 9689, 12623, 14927, 15731, 20737
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* n is 1‑based; invalid for 0 */

    size_t m = sizeof(mersenne_exponents) / sizeof(mersenne_exponents[0]);
    idx_t   cum = 0;                        /* sum of lengths of previous rows */
    size_t  row = 0;                        /* current row index (0‑based) */

    /* Find the row containing the n‑th term. */
    while (row < m && cum + mersenne_exponents[row] < n) {
        cum += mersenne_exponents[row];
        ++row;
    }

    if (row >= m)                          /* n too large for predefined data */
        return (seq_t)0;

    /* 0‑based position within the current row. */
    idx_t pos = n - cum - 1;

    /* The divisor is 2^pos. */
    return pow2(pos);
}
