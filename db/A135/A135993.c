/*
 * A135993  a(0) = 0; a(n) = (floor(n/S2(n))) mod 2 for n >= 1, where S2(n) is
 *          the binary weight of n.
 * https://oeis.org/A135993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n / S2(n)) mod 2 where S2(n) is the binary weight
 *            (popcount) of n. a(0) = 0
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper to count bits of a seq_t value.  Works for any type that
   supports bitwise AND and the comparison to zero. */
static int popcount_seq_t(seq_t x)
{
    int c = 0;
    while (x) {
        x &= x - 1;  /* clear lowest set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* a(0) = 0 */

    int pc = popcount_seq_t((seq_t)n);   /* S2(n) > 0 for n>0 */
    seq_t q = ((seq_t)n) / (seq_t)pc;    /* floor division */
    return q & 1;                        /* modulo 2 */
}
