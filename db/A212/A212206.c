/*
 * A212206  Irregular triangle read by rows: T(n,k) = number of "pat"
 *          permutations of [1..n] with k descents.
 * https://oeis.org/A212206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth term of the triangular array T(n,k) read by
 *            rows, * where * T(n,k) = binomial(2n-2k-1,k) *
 *            binomial(2k,n-k-1) / (2n-2k-1). * The sequence enumerates this
 *            triangle in row‑major order
 * Range:     u8 n <= 25, u16 n <= 62, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* n is 1‑based; n==0 is undefined */
    idx_t cum = 0;
    for (idx_t r = 1; ; ++r) {
        /* length of row r (number of columns k with T(r,k) possibly non‑zero) */
        idx_t len = ((2 * r - 1) / 3) + 1;
        if (n <= cum + len) {
            idx_t k = n - cum - 1;    /* index within the row (0‑based) */
            seq_t a1 = binom(2 * r - 2 * k - 1, k);
            seq_t a2 = binom(2 * k, r - k - 1);
            seq_t denom = (seq_t)(2 * r - 2 * k - 1);
            return (a1 * a2) / denom;
        }
        cum += len;
    }
}
