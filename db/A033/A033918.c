/*
 * A033918  Triangular array in which n-th row consists of the numbers 1^1,
 *          2^2, ... n^n.
 * https://oeis.org/A033918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (n - t(t+1)/2)^(n - t(t+1)/2) where t = floor((-1 +
 *            sqrt(8*n-7))/2)
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is implemented using only integer arithmetic and
   the provided seq_mul helper. */

#include <stdint.h>
#include "../utils/seq_mul.h"

/* Computes base^exp with seq_mul; exp is given as idx_t. */
static seq_t pow_seq_seq_t(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp > 0) {
        res = seq_mul(res, base);
        exp--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1). */
    idx_t t = 0;
    idx_t tri = 0;               /* tri = t*(t+1)/2 */
    /* Find largest t with tri < n.  We keep tri = t*(t+1)/2. */
    while (tri + (t + 1) <= n - 1) {
        t++;
        tri += t;
    }
    idx_t m = n - tri;           /* the row‑internal index, 1 ≤ m ≤ t+1 */
    seq_t base = (seq_t)m;
    return pow_seq_seq_t(base, m);
}
