/*
 * A040968  Continued fraction for sqrt(1000).
 * https://oeis.org/A040968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 31; for n ≥ 1, a(n) = period[(n-1) mod 18]
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued‑fraction expansion of √1000. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[18] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)6, (seq_t)2, (seq_t)2, (seq_t)15, (seq_t)2,
        (seq_t)2, (seq_t)6, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)1, (seq_t)1, (seq_t)62
    };

    if (n == 0)
        return (seq_t)31;

    return period[(size_t)((n - 1) % 18)];
}
