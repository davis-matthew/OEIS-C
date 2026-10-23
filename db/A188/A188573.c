/*
 * A188573  a(n) = coefficient of the sqrt(6) term in (1 + sqrt(2) +
 *          sqrt(3))^n.
 * https://oeis.org/A188573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) - 16*a(n-3) + 8*a(n-4) with initial
 *            values a(0)=0, a(1)=0, a(2)=2, a(3)=6
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the first few terms directly. */
    if (n == 0 || n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    /* Use a rolling buffer of the last four values. */
    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)0;   /* a(n-3) */
    seq_t a2 = (seq_t)2;   /* a(n-2) */
    seq_t a3 = (seq_t)6;   /* a(n-1) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = ((seq_t)4) * a3
           + ((seq_t)4) * a2
           - ((seq_t)16) * a1
           + ((seq_t)8) * a0;

        /* Roll the buffer. */
        a0 = a1;   /* a(n-3) becomes a(n-4) */
        a1 = a2;   /* a(n-2) becomes a(n-3) */
        a2 = a3;   /* a(n-1) becomes a(n-2) */
        a3 = an;   /* a(n)    becomes a(n-1) */
    }
    return an;
}
