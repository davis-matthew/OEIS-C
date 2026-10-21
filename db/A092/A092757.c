/*
 * A092757  Partial sums of round(log_2(n)).
 * https://oeis.org/A092757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{i=1..n} round(log_2(i))
 * Range:     u8 n <= 56, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* round(log_2(i)) is the integer nearest to log_2(i). */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* floor(log2(i)) using the position of the highest bit */
        int k = 63 - __builtin_clzll(i);          /* k >= 0 */
        uint64_t pow = (uint64_t)1 << k;         /* 2^k */

        /* round up if i >= pow * sqrt(2)   <=>   i^2 >= 2 * pow^2   (both sides unsigned) */
        unsigned __int128 lhs = (unsigned __int128)i * i;
        unsigned __int128 rhs = (unsigned __int128)pow * pow * 2u;

        int round_val = k + (lhs >= rhs);        /* k or k+1 */
        sum += (seq_t)round_val;
    }
    return sum;
}
