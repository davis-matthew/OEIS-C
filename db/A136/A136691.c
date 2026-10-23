/*
 * A136691  Final nonzero digit of n! in base 4.
 * https://oeis.org/A136691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n! / 4^⌊v₂(n!)/2⌋) mod 4 * the last non‑zero digit of n!
 *            written in base 4
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;          /* product modulo 4 */
    idx_t cnt2 = 0;          /* total exponent of 2 in n! */

    /* Special case n = 0, 0! = 1 */
    if (n == 0) {
        return 1;
    }

    for (idx_t i = 1; i <= n; ++i) {
        idx_t x = i;
        /* Strip out all factors of 2 from x and count them. */
        while ((x & 1) == 0) {
            ++cnt2;
            x >>= 1;
        }
        /* x is now odd: its value modulo 4 is either 1 or 3. */
        prod = (prod * (x & 3)) & 3;   /* keep result modulo 4 */
    }

    /* After removing all 4‑factors, at most one 2 may remain. */
    if (cnt2 & 1) {
        prod = (prod * 2) & 3;        /* multiply by that extra 2 */
    }

    return prod;
}
