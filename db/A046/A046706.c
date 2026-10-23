/*
 * A046706  a(n) = (1/2)*(n+1)!*Sum_{k=0..floor(n/2)} n^(2k+1)/(2k+1)!.
 * https://oeis.org/A046706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(n+1)!*∑_{k=0..⌊n/2⌋} n^(2k+1)/(2k+1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t m = n / 2;            /* floor(n/2) */
    seq_t n_seq = (seq_t)n;
    seq_t n_sq   = n_seq * n_seq;

    /* T0 = (n+1)! * n */
    seq_t term = factorial(n + 1) * n_seq;

    seq_t ans = term >> 1;      /* term / 2 */
    seq_t rem = term & 1;       /* term % 2 */

    for (idx_t k = 1; k <= m; ++k)
    {
        /* Compute next term: T_k = T_{k-1} * n^2 / ((2k)*(2k+1)) */
        idx_t two_k = 2 * k;
        seq_t denom = (seq_t)two_k * (two_k + 1);   /* (2k)*(2k+1) */

        term = term / denom;
        term = term * n_sq;

        ans += term >> 1;
        rem += term & 1;
    }

    /* rem is even, add the remaining /2 part */
    return ans + (rem >> 1);
}
