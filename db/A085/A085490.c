/*
 * A085490  Number of pairs with two different elements which can be obtained
 *          by selecting unique elements from two sets with n+1 and n^2
 *          elements respectively and n common elements.
 * https://oeis.org/A085490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + n^2 - n
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    /* Compute n^3 + n^2 - n.  All intermediate arithmetic is done in
     * the type seq_t, which is large enough for the intended test range.
     */
    return n_t * n_t * n_t + n_t * n_t - n_t;
}
