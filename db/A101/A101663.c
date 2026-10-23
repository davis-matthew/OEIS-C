/*
 * A101663  Fixed point of morphism 0 -> 01, 1 -> 02, 2 -> 22.
 * https://oeis.org/A101663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 2 * ([a(n) == 2]), a(2n+1) = 1 + ([a(n) > 0]), a(0) = 0
 *            (square brackets denote Iverson brackets, i.e. 1 if the
 *            condition holds, otherwise 0)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t v = a(n >> 1);          /* value at floor(n/2) */

    if ((n & 1) == 0)             /* n is even -> n = 2m */
        return (v == 2) ? 2 : 0;  /* 2 if a(m)==2 else 0 */

    /* n is odd -> n = 2m+1 */
    return 1 + ((v > 0) ? 1 : 0); /* 1 or 2 depending on a(m) */
}
