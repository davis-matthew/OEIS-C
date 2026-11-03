/*
 * A230343  Number of permutations of [2n+3] in which the longest increasing
 *          run has length n+3.
 * https://oeis.org/A230343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3+8*n^2+20*n+19)*(2*n+3)!/(n+5)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 300 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* polynomial part: n^3 + 8*n^2 + 20*n + 19 */
    seq_t n_seq = (seq_t)n;
    seq_t poly = n_seq*n_seq*n_seq + (seq_t)8*n_seq*n_seq +
                 (seq_t)20*n_seq + (seq_t)19;

    if (n == 1)            /* special case where (2n+3)! < (n+5)!  */
        return poly / (seq_t)6;        /* 48/6 = 8 */

    /* compute (2n+3)!/(n+5)! as product of integers from n+6 to 2n+3 */
    seq_t prod = (seq_t)1;
    idx_t start = n + 6;
    idx_t end   = 2*n + 3;          /* may be < start when n == 2, loop skips */

    for (idx_t i = start; i <= end; ++i)
        prod = mul_small(prod, (unsigned int)i);

    return prod * poly;
}
