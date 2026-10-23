/*
 * A030507  Graham-Sloane-type lower bound on the size of a ternary (n,3,7)
 *          constant-weight code.
 * https://oeis.org/A030507
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( binomial(n,7) * 2^7 / (2*n + 1) )
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 87, u64 n <= 3368,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 7)
        return (seq_t)0;            /* undefined for indices below the offset */

    /* Compute C(n,7) multiplicatively using 128‑bit intermediate to avoid overflow */
    unsigned __int128 binom = 1;
    for (idx_t i = 1; i <= 7; ++i) {
        unsigned __int128 num = (unsigned __int128)(n - 7 + i);
        unsigned __int128 den = (unsigned __int128)i;
        binom = binom * num / den;          /* exact division */
    }

    /* Multiply by 2^7 and perform the ceil division by (2*n + 1) */
    unsigned __int128 numerator   = binom * (unsigned __int128)128;
    unsigned __int128 denominator = (unsigned __int128)(2 * n + 1);
    unsigned __int128 result     = (numerator + denominator - (unsigned __int128)1) / denominator;

    return (seq_t)result;
}
