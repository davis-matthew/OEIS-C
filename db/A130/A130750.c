/*
 * A130750  Binomial transform of A010882.
 * https://oeis.org/A130750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 31, u64 n <= 62, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3)
 * Initial values: a(0) = 1, a(1) = 3, a(2) = 8
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)8;

    seq_t a0 = (seq_t)1;  /* a(n-3)   */
    seq_t a1 = (seq_t)3;  /* a(n-2)   */
    seq_t a2 = (seq_t)8;  /* a(n-1)   */
    seq_t an = a2;       /* placeholder for a(n)  */

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)( (3 * a2) - (3 * a1) + (2 * a0) );
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
