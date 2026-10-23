/*
 * A040446  Continued fraction for sqrt(468).
 * https://oeis.org/A040446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0) = 21. For n >= 1, a(n) is multiplicative: a(2^e) = 1 if e =
 *            0 or 1, 2 if e = 2, 42 if e >= 3. a(p^e) = 1 for every odd prime
 *            p. Thus a(n) = 1 for odd n, else let e be the exponent of 2 in
 *            n, and return 1 (e=1), 2 (e=2), or 42 (e>=3)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 21;
    /* Count trailing zeros (the exponent of 2). */
    idx_t e = 0;
    idx_t m = n;
    while ((m & 1) == 0)
    {
        m >>= 1;
        e++;
    }
    /* e == 0 -> n is odd, return 1. */
    if (e == 0 || e == 1) return 1;
    if (e == 2) return 2;
    return 42;
}
