/*
 * A136254  Generator for the finite sequence A053016.
 * https://oeis.org/A136254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3/3 - n^2 + 8n/3 + 4
 * Range:     u8 n <= 5, u16 n <= 59, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* convert index to seq_t to avoid mixed‑type arithmetic */
    seq_t sn = (seq_t)n;

    /* compute n^2 */
    seq_t n2 = sn * sn;          /* n^2 */

    /* compute n*(n^2 + 8) - this quantity is divisible by 3 */
    seq_t temp = sn * (n2 + (seq_t)8);   /* n*(n^2 + 8) */

    /* integer division by 3 (exact) */
    seq_t t = temp / (seq_t)3;   /* (n^3 + 8n)/3 */

    /* final formula: t - n^2 + 4 */
    seq_t result = t - n2 + (seq_t)4;

    return result;
}
