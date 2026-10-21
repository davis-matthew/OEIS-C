/*
 * A062420  Nearest integer to log(n)^log(n).
 * https://oeis.org/A062420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to (ln(n))^(ln(n)) (with 0^0 = 1 for n =
 *            1)
 * Range:     u8 n <= 54, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 (0^0 = 1) */
    if (n == 1)
        return (seq_t)1;

    /* Compute ln(n) as a long double. */
    long double ln_n = logl((long double)n);

    /* Compute (ln n)^(ln n). */
    long double val = powl(ln_n, ln_n);

    /* Round to nearest integer: floor(val + 0.5). */
    unsigned long long rounded = (unsigned long long)(val + 0.5L);

    /* Convert to the generic seq_t type. */
    return (seq_t)rounded;
}
