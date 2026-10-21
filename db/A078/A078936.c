/*
 * A078936  Smallest divisor of n*(n+1)/2 that is >= sqrt(n*(n+1)/2).
 * https://oeis.org/A078936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor of n*(n+1)/2 that is >= sqrt(n*(n+1)/2)
 * Range:     u8 n <= 22, u16 n <= 1561, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_isqrt_seq.h"   /* provides _isqrt_seq(seq_t) */

seq_t a(idx_t n)
{
    /* triangular number T = n*(n+1)/2 */
    seq_t N   = (seq_t)n;              /* cast index to the seq_t type */
    seq_t T   = N * (N + (seq_t)1) / (seq_t)2;

    /* floor(sqrt(T)) as a seq_t */
    seq_t s   = _isqrt_seq(T);

    /* search from sqrt downward for the first divisor */
    seq_t one = 1;
    for (seq_t d = s; d >= one; ) {
        if (T % d == 0)          /* d divides T */
            return T / d;        /* complementary divisor >= sqrt */
        --d;
    }
    /* The loop always finds a divisor (1), but return T as a safe fallback. */
    return T;
}
