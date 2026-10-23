/*
 * A040189  Continued fraction for sqrt(204).
 * https://oeis.org/A040189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=14; for n>=1, a(n) = [3,1,1,6,1,1,3,28][(n-1) mod 8]
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction of sqrt(204) */
    static const seq_t period[8] = { (seq_t)3, (seq_t)1, (seq_t)1,
                                     (seq_t)6, (seq_t)1, (seq_t)1,
                                     (seq_t)3, (seq_t)28 };

    if (n == 0u)
        return (seq_t)14;

    /* Since n>=1, (n-1) is safe, and &7 gives (n-1) mod 8 */
    size_t k = (size_t)((n - 1u) & 7u);
    return period[k];
}
