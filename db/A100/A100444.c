/*
 * A100444  Bisection of A000255.
 * https://oeis.org/A100444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^2+2*n-1)*a(n-1) - 2*n*(2*n-3)*a(n-2); a(0)=1, a(1)=3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)3;  /* a(1) */
    seq_t curr = 0;

    for (idx_t k = 2; k <= n; ++k) {
        uint64_t nk = (uint64_t)k;
        uint64_t c1 = 4ULL * nk * nk + 2ULL * nk - 1ULL;
        uint64_t c2 = 2ULL * nk * (2ULL * nk - 3ULL);
        seq_t coef1 = (seq_t)c1;
        seq_t coef2 = (seq_t)c2;

        curr = coef1 * prev1 - coef2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
