/*
 * A135875  Multiply the positive divisors n in order (starting at 1). a(n) is
 *          the largest such partial product that is <= n.
 * https://oeis.org/A135875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A135875:  a(n) = largest partial product of the positive divisors of n
   taken in ascending order that is <= n.  The divisors are examined in
   increasing order; as soon as a divisor would cause the product to exceed
   n the scan stops.  */

#include <stdint.h>           /* for uint64_t */
#include <stdlib.h>           /* for NULL */

seq_t a(idx_t n)
{
    if (n < 1)          /* the OEIS offset is 1, n==0 is not used */
        return (seq_t)0;

    seq_t prod = 1;     /* current product of divisors examined so far */
    seq_t best = 1;     /* largest product <= n encountered up to now */

    /* Scan all integers from 1 to n; only those that divide n are used. */
    for (idx_t d = 1; d <= n; ++d)
    {
        if (n % d == 0)                /* d is a divisor of n   */
        {
            /* Check if multiplying by d would stay <= n. */
            seq_t limit = (seq_t)(n / d);       /* n/d is integer division */
            if (prod <= limit)
            {
                prod *= d;
                best = prod;
            }
            else                    /* product would exceed n: stop */
                break;
        }
    }
    return best;
}
