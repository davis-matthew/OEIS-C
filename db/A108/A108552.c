/*
 * A108552  Integer values of (1*2*...*k)/(1+2+...+k) = k!/T(k) =
 *          A000142(k)/A000217(k), k>=1.
 * https://oeis.org/A108552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(k-1)!/(k+1) where k is the nth positive integer such
 *            that k==1 or k+1 is composite (i.e., k+1 is not prime)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Find the n-th admissible k (k==1 or k+1 composite) */
    idx_t count = 0;
    idx_t k = 1;
    while (1)
    {
        if (k == 1 || !is_prime(k + 1))
        {
            ++count;
            if (count == n)
                break;
        }
        ++k;
    }

    /* Compute 2*(k-1)!/(k+1) */
    seq_t num = 2;                 /* numerator   : 2*(k-1)!   */
    for (idx_t i = 2; i < k; ++i)
        num *= (seq_t)i;

    seq_t denom = k + 1;           /* denominator : k+1       */
    return num / denom;
}
