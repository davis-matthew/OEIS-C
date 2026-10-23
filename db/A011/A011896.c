/*
 * A011896  a(n) = floor( n*(n-1)*(n-2)/14 ).
 * https://oeis.org/A011896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/14)
 * Range:     u8 n <= 16, u16 n <= 98, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    /* product fits within unsigned long long for all n where the value
       fits in seq_t; for larger n the result may not fit in seq_t anyway. */
    unsigned long long nn  = (unsigned long long)n;
    unsigned long long prod = nn * (nn - 1) * (nn - 2);
    unsigned long long res  = prod / 14u;
    return (seq_t)res;
}
