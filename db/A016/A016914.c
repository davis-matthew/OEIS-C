/*
 * A016914  a(n) = (6*n)^6.
 * https://oeis.org/A016914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n)^6
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 6, u64 n <= 270, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) = (6*n)^6 for the sequence A016914. */
seq_t a(idx_t n)
{
    seq_t base = (seq_t)6 * n;      /* base = 6*n */
    seq_t pow  = base;              /* pow = base^1 */

    /* Multiply 5 more times to obtain base^6 */
    pow *= base;  /* base^2 */
    pow *= base;  /* base^3 */
    pow *= base;  /* base^4 */
    pow *= base;  /* base^5 */
    pow *= base;  /* base^6 */

    return pow;
}
