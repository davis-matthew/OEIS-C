/*
 * A040262  Continued fraction for sqrt(279).
 * https://oeis.org/A040262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A040262: Continued fraction for sqrt(279).
   After the initial term a(0) = floor(sqrt(279)) = 16, the
   continued fraction repeats the period [1,2,2,1,2,2,1,32].
   This implementation returns that pattern for any index n. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial term */
    if (n == 0) return (seq_t)16;

    /* Periodic part */
    static const seq_t period[] = { (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)1,
                                    (seq_t)2, (seq_t)2, (seq_t)1, (seq_t)32 };
    /* n-1 is safe because n>0, modulo period length (8) */
    size_t pos = (size_t)((n - 1) % 8);
    return period[pos];
}
