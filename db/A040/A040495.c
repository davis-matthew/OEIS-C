/*
 * A040495  Continued fraction for sqrt(518).
 * https://oeis.org/A040495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = 22 if n == 0; otherwise a(n) follows the periodic *
 *            continued‑fraction expansion of sqrt(518): [1,3,6,3,1,44]
 *            repeating
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)22;

    /* periodic part of the continued fraction for √518 */
    static const seq_t period[6] = {
        (seq_t)1,
        (seq_t)3,
        (seq_t)6,
        (seq_t)3,
        (seq_t)1,
        (seq_t)44
    };

    size_t k = (size_t)((n - 1) % 6);
    return period[k];
}
