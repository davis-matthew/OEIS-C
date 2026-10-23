/*
 * A195462  1!*2!*3!*...*n!*H(n); H(n) the n-th harmonic number.
 * https://oeis.org/A195462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1!*2!*3!*...*n!*H(n) (H(n) = harmonic number)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* the OEIS offset is 1; behaviour below 1 is irrelevant */

    seq_t a_val = 1;        /* a(1) = 1 */
    seq_t fact  = 1;        /* current factorial (k! for current k)          */
    seq_t prod  = 1;        /* product of factorials up to previous k (k-1)!   */

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t fact_prev = fact;
        seq_t prod_prev = prod;

        /* k! = fact_prev * k */
        fact = fact * (seq_t)k;

        /* product of factorials up to k */
        prod = prod * fact;

        /* recurrence: a(k) = a(k-1)*k! + prod(k-1)*(k-1)! */
        a_val = a_val * fact + prod_prev * fact_prev;
    }

    return a_val;        /* a(n) */
}
