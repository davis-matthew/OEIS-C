/*
 * A120529  First differences of successive generalized meta-Fibonacci numbers
 *          A120507.
 * https://oeis.org/A120529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120529: first differences of successive generalized meta‑Fibonacci
 * numbers A120507.
 *
 * The generating function is
 *    D(z) = z * Π_{i≥1} (1 + z^{[i]} + z^{2[i]} + z^{3[i]}),
 * where [i] = (4^i – 1)/3.  Consequently the coefficient a(n) is
 * 1 if the integer n–1 can be expressed as
 *    n–1 = Σ_{i≥1} k_i · [i]   with 0 ≤ k_i ≤ 3,
 * and 0 otherwise.  Because [i+1] = 4·[i] + 1, each integer has a unique
 * such representation (or none).  The function below tests this
 * representability by a greedy algorithm.
 *
 *   a(n) = { 1  if n‑1 is representable,
 *           { 0  otherwise.
 */

#include <stdlib.h>   /* for size_t, NULL */

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n==0; return 0 */
        return 0;

    /* m = n-1 (use idx_t for arithmetic to avoid overflow of small seq_t) */
    idx_t m = (idx_t)n - 1;

    /* generate all weights [i] ≤ m: 1, 5, 21, 85, ...   */
    idx_t weight[64];            /* max needed is small, 64 is safe */
    size_t wcnt = 0;
    idx_t w = 1;                 /* [1] */
    while (w <= m) {
        weight[wcnt++] = w;
        /* next weight: [i+1] = 4·[i] + 1 */
        w = 4 * w + 1;
        /* safety for overflow – stops when w wraps to 0 */
        if (w == 0)
            break;
    }

    /* greedy decomposition: start from the largest weight */
    idx_t remaining = m;
    for (size_t i = wcnt; i > 0; --i) {
        idx_t wgt = weight[i - 1];
        idx_t digit = remaining / wgt;
        if (digit > 3)      /* allowed digits are 0..3 */
            digit = 3;
        remaining -= digit * wgt;
    }

    return (seq_t)(remaining == 0);
}
