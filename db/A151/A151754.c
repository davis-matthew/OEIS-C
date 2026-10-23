/*
 * A151754  Number of n-digit numbers that are divisible by 5^n.
 * https://oeis.org/A151754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((9/10)*2^n) Recurrence: a(1) = 1; a(n+1) = 2*a(n) +
 *            ((n%4==1) || (n%4==2)) The recurrence follows from
 *            floor((9/10)*2^n) = floor((9/10)*2^(n-1)) * 2 plus a corrective
 *            0 or 1 depending on n mod 4
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cur = 1;               /* a(1) */
    for (idx_t i = 1; i < n; ++i)
    {
        /* Add 1 if n modulo 4 is 1 or 2, otherwise add 0 */
        seq_t inc = ((i % 4 == 1) || (i % 4 == 2)) ? (seq_t)1 : (seq_t)0;
        cur = cur * 2 + inc;
    }
    return cur;
}
