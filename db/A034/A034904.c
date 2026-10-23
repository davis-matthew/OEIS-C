/*
 * A034904  Related to sept-factorial numbers A045754.
 * https://oeis.org/A034904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A034904: a(n) = 7^(n-1) * A045754(n) / n!
   Recurrence: a(1) = 1
                a(n) = 7 * (7*n-6) / n * a(n-1)   (n >= 2) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, return 0 */
    seq_t result = 1;                /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor = 7 * ((seq_t)7 * (seq_t)i - 6);   /* 7*(7*i-6) */
        result = result * factor / i;
    }
    return result;
}
