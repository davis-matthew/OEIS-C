/*
 * A061050  Denominator of 1/64 - 1/n^2.
 * https://oeis.org/A061050
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 1/64 - 1/n². The reduced denominator is
 *            (64 n²) / gcd(n² − 64, 64 n²)
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for indices below the offset */
    if (n < 8) return 0;

    /* Work in 128‑bit to avoid overflow for the intermediate products. */
    unsigned __int128 nn   = n;
    unsigned __int128 num  = nn * nn - (unsigned __int128)64;   /* n² − 64   */
    unsigned __int128 den  = (unsigned __int128)64 * nn * nn;   /* 64 n²     */

    /* Compute gcd(num, den) using the Euclidean algorithm on 128‑bit ints. */
    unsigned __int128 a128 = num, b128 = den;
    while (b128 != 0)
    {
        unsigned __int128 r = a128 % b128;
        a128 = b128;
        b128 = r;
    }
    unsigned __int128 g     = a128;            /* gcd */
    unsigned __int128 result = den / g;        /* reduced denominator */

    return (seq_t)result;
}
