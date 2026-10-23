/*
 * A109594  n followed by n^3 followed by n^2.
 * https://oeis.org/A109594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)/3)^((3*(n mod 3)^2-5*(n mod 3)+4)/2)
 * Range:     u8 n <= 19, u16 n <= 121, u32 n <= 4876, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* offset is 1 */

    /* compute base = floor((n+2)/3) */
    seq_t base = ((seq_t)n + 2) / 3;

    /* compute exponent based on n mod 3 */
    idx_t r = n % 3;
    seq_t exp = (r == 0) ? 2 : (r == 1) ? 1 : 3;

    /* compute base^exp */
    seq_t result = 1;
    while (exp > 0) {
        result *= base;
        exp--;
    }

    return result;
}
