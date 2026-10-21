/*
 * A076125  Define r(k) as follows: r(1)=n, r(k+1) = r(k)
 *          +(-1)^k*sign(r(k)-k)*k; then abs(r(k)) = n for at least one k>1
 *          (there could be 2 values for k > 1). Sequence gives the smallest
 *          value of k > 1 such that abs(r(k)) = n.
 * https://oeis.org/A076125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 1 such that |r_k| = n, where r_1 = n and
 *            r_{k+1} = r_k + (-1)^k * sign(r_k - k) * k. sign(x) = 1 if x >
 *            0, -1 if x < 0, and 0 if x = 0
 * Range:     u8 n <= 50, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset: the sequence starts at n = 1. */
    if (n < 1)
        return (seq_t)0;

    long long r = (long long)n;      /* current r_k, use 64‑bit signed to avoid overflow */
    idx_t k = 1;                     /* r_1 has been set, next index will be r_2 (k+1) */

    for (;; ++k)
    {
        /* Compute sign(r_k - k) */
        int sign;
        if (r > (long long)k)          sign = 1;
        else if (r < (long long)k)     sign = -1;
        else                           sign = 0;

        /* (-1)^k : +1 for even k, -1 for odd k */
        int pow = (k & 1) ? -1 : 1;

        long long delta = sign * pow * (long long)k;
        r += delta;

        /* We now have r_{k+1}. Check if |r| == n. */
        if (r == (long long)n || r == -(long long)n)
            return (seq_t)(k + 1);      /* the index k+1 is the required a(n) */
    }
}
