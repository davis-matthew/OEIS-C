/*
 * A049447  Stirling numbers of second kind: 9th column of Stirling2 triangle
 *          A008277.
 * https://oeis.org/A049447
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = S(n,9) where S(n,k) is a Stirling number of the second
 *            kind. The recurrence used is S(n,k) = S(n-1,k-1) + k * S(n-1,k),
 *            with S(0,0)=1 and S(n,0)=0. This function computes S(n,9) for
 *            any n ≥ 9 (values for n < 9 are defined as 0)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 16, u64 n <= 26, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */

enum { K = 9 };   /* fixed column index */

seq_t a(idx_t n)
{
    /* local array storing S(n,k) for k=0..K of the current row n */
    seq_t S[K + 1] = {0};

    S[0] = 1;   /* S(0,0) */

    for (idx_t i = 1; i <= n; ++i) {
        idx_t maxk = (i < K) ? i : K;
        for (idx_t k = maxk; k >= 1; --k) {
            S[k] = S[k - 1] + (seq_t)k * S[k];
        }
        S[0] = 0;  /* S(n,0) = 0 for n > 0 */
    }

    return S[K];
}
