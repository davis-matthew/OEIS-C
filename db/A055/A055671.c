/*
 * A055671  Number of prime Hurwitz quaternions of norm n.
 * https://oeis.org/A055671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 24 if n = 2; a(n) = 24*(n+1) if n is an odd prime; a(n) =
 *            0 otherwise
 * Range:     u8 n <= 10, u16 n <= 2730, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    if (n == 2)
        return 24;

    if ((n & 1) == 0)           /* even n > 2 cannot be prime */
        return 0;

    /* simple trial division for odd n */
    for (idx_t d = 3; d*d <= n; d += 2)
        if (n % d == 0)
            return 0;

    /* n is an odd prime */
    return (seq_t)24 * ((seq_t)n + 1);
}
