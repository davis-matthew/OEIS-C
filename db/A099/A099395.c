/*
 * A099395  One if odd part of n is 3, zero otherwise.
 * https://oeis.org/A099395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the odd part of n is 3, otherwise 0
 * Range:     u8 n <= 9618, u16 n <= 9618, u32 n <= 9618, u64 n <= 9618,
 *            u128 n <= 9618, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 ⇔ n = 3·2^k for some k ≥ 0, else a(n) = 0 */
seq_t a(idx_t n)
{
    /* n ≥ 1 by the OEIS offset */
    seq_t odd = (seq_t)n;

    /* remove all factors of 2 */
    while ((odd & 1) == 0)
        odd >>= 1;

    return (odd == 3) ? 1 : 0;
}
