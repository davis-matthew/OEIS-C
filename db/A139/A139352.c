/*
 * A139352  Let the binary expansion of n be n = Sum_{k} 2^{r_k}, let e(n) be
 *          the number of r_k's that are even, o(n) the number that are odd;
 *          sequence gives o(n).
 * https://oeis.org/A139352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of set bits in the binary expansion of n that
 *            occur at odd positions (counting the least‑significant bit as
 *            position 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;
    seq_t count = 0;
    while (x) {
        if (x & 2) ++count;   /* bit at odd position */
        x >>= 2;              /* move to next odd position */
    }
    return count;
}
