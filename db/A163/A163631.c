/*
 * A163631  Partial sums of the odd nonprimes, A014076.
 * https://oeis.org/A163631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the first n odd nonprimes (1, 9, 15, 21, …)
 * Range:     u8 n <= 10, u16 n <= 199, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(1)=1, a(2)=10, a(3)=25, …) */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  A call with n==0 is not
       required, but we return 0 safely. */
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;          /* number of odd nonprimes found */
    seq_t  sum   = 0;          /* running sum of odd nonprimes */

    /* Inspect odd numbers starting from 1.  1 is treated as a nonprime. */
    for (idx_t cur = 1; ; cur += 2)
    {
        if (cur == 1 || !is_prime(cur))
        {
            sum += (seq_t)cur;
            ++count;
            if (count == n)
                return sum;
        }
    }
}
