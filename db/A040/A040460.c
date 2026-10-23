/*
 * A040460  Continued fraction for sqrt(482).
 * https://oeis.org/A040460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0) = 21. For n≥1: a is multiplicative: a(2) = 20, a(2^e) = 42
 *            for e≥2, a(p^e) = 1 for any odd prime p. Equivalently: - if n is
 *            odd → a(n) = 1 - if n is even and not divisible by 4 → a(n) = 20
 *            - if n is divisible by 4 → a(n) = 42
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)21;
    if ((n & (idx_t)1) != 0)
        return (seq_t)1;

    /* Count the exponent of 2 in n */
    idx_t e = 0;
    idx_t t = n;
    while ((t & (idx_t)1) == 0) {
        t >>= 1;
        e++;
    }

    if (e == 1)
        return (seq_t)20;
    else
        return (seq_t)42;
}
