/*
 * A211425  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w^2+4*x*y=0.
 * https://oeis.org/A211425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4n + 1) + 4 * Σ_{u=1}^{⌊n/2⌋} C(u²) where C(s) = number
 *            of positive divisors d of s with d ≤ n and s/d ≤ n (ordered
 *            pairs of divisors)
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle n == 0 directly (value 1) */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;                     /* Σ C(u²)   */
    idx_t max_u = n / 2;                /* u ranges 1..floor(n/2) */

    for (idx_t u = 1; u <= max_u; ++u)
    {
        seq_t s = (seq_t)u * (seq_t)u; /* u²   */

        /* Enumerate divisors d of u² up to sqrt(u²)=u.
           Each divisor gives a pair (d, e) with e = u² / d. */
        for (idx_t d = 1; d <= u; ++d)
        {
            seq_t d_seq = (seq_t)d;
            if (s % d_seq == 0)           /* d divides s */
            {
                seq_t e = s / d_seq;
                /* count the ordered pair (d, e) only if both ≤ n */
                if (d_seq <= (seq_t)n && e <= (seq_t)n)
                {
                    if (d_seq == e)       /* square root, single ordered pair */
                        sum += 1;
                    else                    /* distinct divisors, two ordered pairs */
                        sum += 2;
                }
            }
        }
    }

    seq_t result = (seq_t)(4 * (seq_t)n + 1) + 4 * sum;
    return result;
}
