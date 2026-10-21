/*
 * A087055  Largest square number less than 2*n^2.
 * https://oeis.org/A087055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n*sqrt(2))^2
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a seq_t value using only integer arithmetic. */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low  = 0;
    seq_t high = x;          /* high is at least as big as the root */

    while (low < high) {
        seq_t mid = low + ((high - low + 1) >> 1);
        /* mid * mid <= x  iff  mid <= x / mid   (avoids overflow). */
        if (mid == 0 || mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* OEIS A087055: largest square < 2*n^2 */
seq_t a(idx_t n)
{
    seq_t N        = (seq_t)n;
    seq_t two_n_sq = N * N;       /* N² */
    two_n_sq *= (seq_t)2;         /* 2*N² */
    seq_t m = isqrt_seq(two_n_sq);/* floor(sqrt(2*n²)) */
    return m * m;                 /* square of that integer */
}
