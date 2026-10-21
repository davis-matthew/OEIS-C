/*
 * A099551  Odd part of n modulo 10. Final digit of A000265(n).
 * https://oeis.org/A099551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (odd part of n) mod 10
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset start is 1; safe fallback */

    idx_t m = n;
    while ((m & 1) == 0)
        m >>= 1;          /* remove all factors of 2 */

    return (seq_t)(m % 10);   /* the odd part modulo 10 */
}
