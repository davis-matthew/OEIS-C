/*
 * A065168  Permutation t->t-1 of Z, folded to N.
 * https://oeis.org/A065168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 2, a(n) = n + 2 for odd n, a(n) = n – 2 for
 *            even n ≥ 4
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t un = (seq_t)n;           /* promote index to seq_t */
    if (un == 2)                   /* special case */
        return (seq_t)1;

    if (un & 1)                    /* odd n */
        return un + 2;             /* a(n) = n + 2 */

    return un - 2;                 /* even n ≥ 4 : a(n) = n – 2 */
}
