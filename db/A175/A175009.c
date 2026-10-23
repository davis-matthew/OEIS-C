/*
 * A175009  Triangle read by rows, antidiagonals of an array formed from
 *          variants of A001318, generalized pentagonal numbers.
 * https://oeis.org/A175009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   a001318
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001318.h"
/*
 * Formula: a(n) is the antidiagonal reading of a triangular array T(k,j).
 *   T(1,j) = A001318(j), the nth generalized pentagonal number (offset 1):
 *     if j is odd:  a = ((j+1)/2) * (3*(j+1)/2 - 1) / 2
 *     if j is even: a = ((j)/2)   * (3*(j)/2   + 1) / 2
 *   For k > 1:  T(k,j) = 1 + k * Σ_{t=1}^{j-1} Q(t), where
 *     Q(t) = A026741(t):
 *       if t is odd:  Q(t) = (t+1)/2
 *       if t is even: Q(t) = t + 1
 *
 * The sequence a(n) is obtained by reading T(k,j) along antidiagonals:
 *   (1,1), (2,1), (1,2), (3,1), (2,2), (1,3), ...
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* off‑by‑zero guard */

    /* find diagonal d such that d*(d+1)/2 >= n */
    idx_t d = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++d;
        cum += d;                      /* cum = d(d+1)/2 */
    }
    idx_t prev = cum - d;              /* elements before diagonal d */
    idx_t pos = n - prev - 1;          /* position inside diagonal, 0‑based */
    idx_t k = d - pos;                 /* row index   */
    idx_t j = pos + 1;                 /* column index */

    if (k == 1)
        return a001318(j);

    /* compute partial sum of Q(t) for t = 1 .. j-1 */
    seq_t sum = 0;
    for (idx_t t = 1; t < j; ++t) {
        if (t & 1)                     /* t odd */
            sum += ((seq_t)(t + 1)) / 2;
        else                           /* t even */
            sum += (seq_t)(t + 1);
    }
    return (seq_t)1 + (seq_t)k * sum;   /* T(k,j) for k > 1 */
}
