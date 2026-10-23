/*
 * A136699  Final nonzero digit of n! in base 13.
 * https://oeis.org/A136699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n! / 13^{v13(n!)}) mod 13, i.e. the final non‑zero digit
 *            of n! in base 13
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* 0! = 1 */
    seq_t prod = 1;                    /* product modulo 13, contains no factor 13 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t x = (seq_t)i;            /* copy i into seq_t for manipulation */
        /* remove all factors of 13 from x */
        while (x % 13 == 0)
            x /= 13;
        /* accumulate product modulo 13 */
        prod = (prod * (x % 13)) % 13;
    }
    return prod;
}
