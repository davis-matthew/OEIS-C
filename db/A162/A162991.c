/*
 * A162991  The first right hand column of triangle A162990.
 * https://oeis.org/A162991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+1)! / n)^2 Simplified integer form: a(n) = ((n+1) *
 *            (n-1)!)^2 for n >= 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* undefined for n = 0; return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* compute (n-1)! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n - 1; ++i) {
        fact *= (seq_t)i;
    }

    /* tmp = (n+1) * (n-1)! */
    seq_t tmp = fact * (seq_t)(n + 1);

    /* a(n) = tmp^2 */
    return tmp * tmp;
}
