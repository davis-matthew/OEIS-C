/*
 * A072400  (Factors of 4 removed from n) modulo 8.
 * https://oeis.org/A072400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n with all factors of 4 removed) mod 8 Equivalent to
 *            a(n) = (n / 4^v4(n)) mod 8, where v4(n) is the largest k with
 *            4^k dividing n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                    /* offset starts at 1, but guard against 0 */
        return 0;

    idx_t t = n;
    /* Remove all powers of 4 by repeatedly dividing by 4 (i.e., shifting right by 2) */
    while ((t & 3ULL) == 0)        /* t % 4 == 0 for unsigned types */
        t >>= 2;

    /* Now t is not divisible by 4; take modulo 8 which is just the lowest 3 bits */
    return (seq_t)(t & 7ULL);
}
