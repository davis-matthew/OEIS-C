/*
 * A036353  Square pentagonal numbers.
 * https://oeis.org/A036353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:
 *   a(0) = 0
 *   a(1) = 1
 *   a(2) = 9801
 *   a(n) = 9602*a(n-1) - a(n-2) + 200   for n >= 3
 *
 * This recurrence generates the OEIS sequence A036353 – square pentagonal numbers.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9801;

    seq_t a_nm2 = (seq_t)1;       /* a(1) */
    seq_t a_nm1 = (seq_t)9801;    /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i)
    {
        curr = (seq_t)9602 * a_nm1 - a_nm2 + (seq_t)200;
        a_nm2 = a_nm1;
        a_nm1 = curr;
    }
    return curr;
}
