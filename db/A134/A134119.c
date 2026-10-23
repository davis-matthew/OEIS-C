/*
 * A134119  a(n) = floor(n^2/10) - floor((n-1)^2/10).
 * https://oeis.org/A134119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2/10) - floor((n-1)^2/10)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n >= 10: a(n) = a(n-10) + 2, with base pattern 0,0,0,0,1,1,1,1,2,2 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* base pattern for n mod 10 */
    static const seq_t base[10] = {0,0,0,0,1,1,1,1,2,2};

    /* remainder n%10 */
    int r = (int)(n % 10);
    idx_t q = n / 10;          /* integer division */

    /* compute value = base[r] + 2*q, converting to seq_t */
    seq_t value = base[r] + (seq_t)(q * 2);
    return value;
}
