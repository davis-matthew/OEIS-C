/*
 * A163662  A020988 written in base 2.
 * https://oeis.org/A163662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{k=1..n} 10^(2*k-1) = 100*a(n-1) + 10, a(0)=0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = 0;
    /* Iterate n times, building the decimal string consisting of n copies of "10". */
    for (idx_t i = 1; i <= n; ++i) {
        val = val * 100 + 10;   /* shift by two decimal digits and append "10" */
    }
    return val;
}
