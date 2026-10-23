/*
 * A040022  Continued fraction for sqrt(28).
 * https://oeis.org/A040022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 5; for n >= 1, a(n) = {3,2,3,10}[(n-1) mod 4]
 * Range:     u8 n <= 8602, u16 n <= 8602, u32 n <= 8602, u64 n <= 8602,
 *            u128 n <= 8602, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* The continued‑fraction coefficients for sqrt(28) are periodic
       with period [3, 2, 3, 10] after the first term (5).  */
    if (n == 0)
        return (seq_t)5;

    /* Compute the index in the period.  Using (seq_t) ensures that
       the modulo operation is performed on an unsigned type that
       matches the size of seq_t.  */
    const seq_t period[4] = { (seq_t)3, (seq_t)2, (seq_t)3, (seq_t)10 };
    idx_t m = n - 1;          /* shift so that the first period element
                                 corresponds to n = 1 */
    size_t r = (size_t)(m % 4);  /* r in {0,1,2,3} */
    return period[r];
}
