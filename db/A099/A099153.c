/*
 * A099153  Iterated heptagonal numbers (A000566), starting at 7.
 * https://oeis.org/A099153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 7, a(n) = a(n-1) * (5*a(n-1) - 3) / 2 for n ≥
 *            2. This implements the iterated heptagonal numbers of OEIS
 *            A099153. Intermediate products are split by dividing one factor
 *            by 2 first to avoid overflow when seq_t is an unsigned 64‑bit
 *            type
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* seq_t and idx_t are provided by the build system. */

seq_t a(idx_t n)
{
    /* a(0) and a(1) are fixed by definition. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t val = (seq_t)7;           /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        /* tmp = 5*val - 3  (all operations in seq_t) */
        seq_t tmp = ((seq_t)5 * val) - (seq_t)3;

        /* Multiply val and tmp, dividing one factor by 2 first
           to keep the intermediate product within seq_t limits. */
        if ((val & (seq_t)1) == (seq_t)0) {
            /* val is even */
            val = (val / (seq_t)2) * tmp;
        } else {
            /* tmp is even (product is always even) */
            tmp = tmp / (seq_t)2;
            val = val * tmp;
        }
    }
    return val;
}
