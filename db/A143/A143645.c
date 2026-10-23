/*
 * A143645  Aliquot sequence starting at 24.
 * https://oeis.org/A143645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = A001065(a(n)), a(0) = 24 (the sum of proper divisors of
 *            a(n))
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"


static seq_t aliquot_next(seq_t x)
{
    /* for 0 or 1 the sum of proper divisors is 0 */
    if (x <= (seq_t)1)
        return (seq_t)0;

    seq_t sum  = (seq_t)1;                /* 1 always divides x (>1) */
    seq_t limit = floor_sqrt(x);

    for (seq_t i = (seq_t)2; i <= limit; ++i) {
        if (x % i == (seq_t)0) {
            sum += i;
            seq_t other = x / i;
            if (other != i)
                sum += other;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* a(0) = 24 */
    if (n == (idx_t)0)
        return (seq_t)24;

    seq_t cur = (seq_t)24;
    for (idx_t step = (idx_t)1; step <= n; ++step) {
        cur = aliquot_next(cur);
        /* once we hit 0 the sequence stays 0 */
        if (cur == (seq_t)0)
            return (seq_t)0;
    }
    return cur;
}
