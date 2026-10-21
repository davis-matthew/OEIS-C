/*
 * A071111  a(n) is the least integer x such that there exists an integer in
 *          the open interval (x/(i+1), x/i) for i= n-1, n-2 ..., 3, 2, 1.
 * https://oeis.org/A071111
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n^2 - n*c(n) + floor(c(n)^2 / 4) + 1, where c(n) =
 *            floor(sqrt(4n-5)). This holds for all n >= 2
 * Range:     u8 n <= 19, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt_seq.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;  /* undefined for n < 2, behaviour does not matter */

    seq_t N = (seq_t)n;
    seq_t t = (seq_t)4 * N - (seq_t)5;          /* 4n - 5 */
    seq_t c = floor_sqrt_seq(t);              /* c(n) = floor(sqrt(4n-5)) */

    seq_t n_sq = N * N;                       /* n^2 */
    seq_t n_c  = N * c;                       /* n*c */
    seq_t c_sq = c * c;                       /* c^2 */

    seq_t res = n_sq - n_c + (c_sq / (seq_t)4) + (seq_t)1;
    return res;
}
