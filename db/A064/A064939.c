/*
 * A064939  a(n) = Sum_{i=1..omega(n)} i*p_i, where {p_i}, i=1..omega(n) is
 *          the increasing sequence of prime divisors of n, where omega is the
 *          number of distinct prime factors of n (A001221).
 * https://oeis.org/A064939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, q|n, p <= q} q
 * Range:     u8 n <= 253, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) as the sum of i * p_i where p_i are the distinct
   prime divisors of n in increasing order. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1u)               /* a(1) = 0 by definition */
        return (seq_t)0;

    idx_t temp = n;
    seq_t sum  = 0;
    seq_t pos  = 1;          /* 1‑based position of the prime divisor */

    /* handle factor 2 once */
    if ((temp & 1u) == 0) {          /* temp % 2 == 0 */
        sum += pos * (seq_t)2;
        pos++;
        while ((temp & 1u) == 0)
            temp >>= 1;
    }

    /* odd factors */
    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            sum += pos * (seq_t)d;
            pos++;
            while (temp % d == 0)
                temp /= d;
        }
    }

    /* any remaining prime factor greater than sqrt() */
    if (temp > 1) {
        sum += pos * (seq_t)temp;
    }

    return sum;
}
