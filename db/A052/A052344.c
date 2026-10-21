/*
 * A052344  Number of ways to write n as the unordered sum of two nonzero
 *          triangular numbers.
 * https://oeis.org/A052344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered representations of n as a sum of two
 *            * non‑zero triangular numbers. That is, * a(n) = #{(i,j) | 1 ≤ i
 *            ≤ j, T_i + T_j = n}, * where T_k = k(k+1)/2
 * Range:     u8 n <= 234, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Trivial cases: need at least 1+1 = 2. */
    if (n < 2)
        return 0;

    /* Find the largest k such that T_k ≤ n. */
    idx_t k = 1;
    while (1) {
        seq_t t = ((seq_t)k) * (((seq_t)k) + 1) / 2;
        if (t > n)
            break;
        ++k;
    }
    idx_t m = k - 1;                /* number of triangular numbers ≤ n */

    if (m == 0)
        return 0;

    /* Allocate and fill the triangular numbers array. */
    seq_t *tri = (seq_t *)malloc(sizeof(seq_t) * (size_t)m);
    if (!tri)
        return 0;                  /* out of memory → zero */
    for (idx_t i = 1; i <= m; ++i)
        tri[i - 1] = ((seq_t)i) * (((seq_t)i) + 1) / 2;

    /* Two‑pointer search for unordered pairs. */
    idx_t i = 0;
    idx_t j = m - 1;
    seq_t count = 0;
    seq_t target = (seq_t)n;

    while (i <= j) {
        seq_t sum = tri[i] + tri[j];
        if (sum < target) {
            ++i;
        } else if (sum > target) {
            if (j == 0) break;
            --j;
        } else { /* sum == target */
            ++count;
            ++i;
            if (j == 0) break;
            --j;
        }
    }

    free(tri);
    return count;
}
