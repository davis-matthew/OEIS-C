/*
 * A053733  a(n) = ceiling(binomial(n,9)/n).
 * https://oeis.org/A053733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(binomial(n,9)/n)
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 83, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 9)
        return (seq_t)0;

    /* Use 128‑bit arithmetic for the numerator to avoid overflow */
    unsigned __int128 prod = 1;
    for (idx_t i = 1; i <= 8; ++i)
        prod *= (unsigned __int128)(n - i);

    const unsigned __int128 denom = 362880ULL;   /* 9! */
    /* Ceil division */
    unsigned __int128 q = prod / denom;
    unsigned __int128 r = prod % denom;
    return (seq_t)(q + (r != 0));
}
