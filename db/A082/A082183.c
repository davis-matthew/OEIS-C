/*
 * A082183  Smallest k > 0 such that T(n) + T(k) = T(m), for some m, T(i)
 *          being the triangular numbers, n > 1.
 * https://oeis.org/A082183
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A082183
 *  a(n) = smallest k > 0 such that there exists m with
 *        T(n)+T(k)=T(m),  where T(x)=x*(x+1)/2.
 *
 *  The algorithm searches for the smallest integer t>0 such that
 *  1+4*t*(2*n+t+1) is a perfect square.  For a found t the
 *  corresponding k is (sqrt(1+4*t*(2*n+t+1))-1)/2.
 *  The first t that yields a perfect square gives the minimal k
 *  because the expression grows monotonically with t.
 *
 *  Complexity is O(k) with a small constant; for n up to ten
 *  thousand this is far below two seconds.
 */

#include "../utils/floor_sqrt_seq.h"

/* compute a(n) */
seq_t a(idx_t n)
{
    /* trivial: for n < 2 the behaviour is irrelevant */
    if (n < 2) return 0;

    const seq_t one  = (seq_t)1;
    const seq_t two  = (seq_t)2;
    const seq_t four = (seq_t)4;

    for (idx_t t = 1; ; ++t)
    {
        /* temp = 2*n + t + 1 */
        seq_t temp = two * (seq_t)n + (seq_t)t + one;

        /* D = 1 + 4*t*temp  */
        seq_t D = one + four * (seq_t)t * temp;

        /* s = floor(sqrt(D))  */
        seq_t s = floor_sqrt_seq(D);

        /* check for perfect square */
        if (s * s == D)
        {
            seq_t k = (s - one) / two;   /* k = (s-1)/2 */
            if (k > 0) return k;
        }
    }

    /* unreachable – a solution always exists */
    return 0;
}
