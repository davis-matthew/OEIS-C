/*
 * A131680  a(n) = (Product_{i=1..10} n^i+i)/10!.
 * https://oeis.org/A131680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∏_{i=1..10} (n^i + i)) / 10!
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define FACT10 3628800u

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;   /* convert index to the integer type used for values   */
    seq_t pow  = 1;           /* current n^i                                         */
    seq_t prod = 1;           /* product of the terms                                */

    for (idx_t i = 1; i <= 10u; ++i)
    {
        pow  *= sn;                 /* pow = n^i                                          */
        prod *= (pow + (seq_t)i);   /* multiply by (n^i + i)                              */
    }

    prod /= FACT10;                /* divide by 10! (already known)                      */
    return prod;
}
