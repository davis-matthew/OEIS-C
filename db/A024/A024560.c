/*
 * A024560  a(n) = Sum_{k=1..n} floor(1/{k*sqrt(6)}) where {x} := x -
 *          floor(x).
 * https://oeis.org/A024560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(1 / {k*sqrt(6)}) where {x} = x -
 *            floor(x)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isqrt_seq.h"

/* Helper: compute floor(1/{k*sqrt(6)}) using integer arithmetic */
static inline seq_t _term(idx_t k)
{
    seq_t K = (seq_t)k;
    /* N = 6*k*k */
    seq_t N = (seq_t)6 * K * K;
    /* m = floor(sqrt(N)) */
    seq_t m = isqrt_seq(N);
    /* r = N - m^2, 1 <= r <= 2*m */
    seq_t r = N - m * m;
    /* s = 2*m */
    seq_t s = m * 2;
    /* initial estimate t = floor(s/r) */
    seq_t t = s / r;
    /* adjust upward if needed:
       t == floor((sqrt(N)+m)/r) ?
       Condition: (t+1)*r <= sqrt(N)+m
       => ((t+1)*r - m)^2 <= N
    */
    seq_t diff = (t + 1) * r - m;
    if (diff * diff <= N) {
        t++;
    }
    return t;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += _term(k);
    }
    return sum;
}
