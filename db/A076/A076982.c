/*
 * A076982  Number of triangular numbers that divide the n-th triangular
 *          number.
 * https://oeis.org/A076982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{k : 1 ≤ k ≤ n and k(k+1) | n(n+1)}|
 * Range:     u8 n <= 16, u16 n <= 689, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count triangular numbers that divide the n‑th triangular number. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by contract */
    seq_t nn  = (seq_t)n;
    seq_t prod = nn * (nn + 1);          /* n(n+1); denominator of triangular numbers */
    seq_t count = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t m  = kk * (kk + 1);        /* k(k+1)   */
        if (prod % m == 0) {
            ++count;
        }
    }
    return count;
}
