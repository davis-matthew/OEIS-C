/*
 * A090768  a(n) = 4*7^(n^2+2n+1)*Product_{j=1..n} (49^j-1).
 * https://oeis.org/A090768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*7^((n+1)^2)*Π_{j=1..n}(49^j-1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=28; a(n)=a(n-1)*7*49^n*(49^n-1)  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;

    seq_t result = (seq_t)28;
    seq_t pow49 = (seq_t)1;              /* 49^0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* pow49 becomes 49^i */
        pow49 *= (seq_t)49;

        /* term = 7 * 49^i * (49^i - 1)  */
        seq_t term = pow49 - (seq_t)1;
        term *= pow49;
        term *= (seq_t)7;

        result *= term;
    }

    return result;
}
