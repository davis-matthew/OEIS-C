/*
 * A016948  a(n) = (6*n + 3)^4.
 * https://oeis.org/A016948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 3)^4
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 42, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to avoid premature truncation. */
    seq_t ns = (seq_t)n;

    /* Compute the base value 6*n + 3. */
    seq_t base = ns * (seq_t)6 + (seq_t)3;

    /* Compute the fourth power using two squarings. */
    seq_t sq = base * base;    /* base^2 */
    return sq * sq;            /* base^4 */
}
