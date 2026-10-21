/*
 * A060105  Sixth column (m=5) of triangle A060102.
 * https://oeis.org/A060105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4)*(8*n^2+64*n+45)*binomial(n+7,7)/(4*45)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 25, u64 n <= 266, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate arithmetic to avoid overflow for all
     * values that fit in the target seq_t type. */
    unsigned __int128 tn = (unsigned __int128)n;

    /* t = 8*n^2 + 64*n + 45 */
    unsigned __int128 t = (unsigned __int128)8 * tn * tn
                       + (unsigned __int128)64 * tn
                       + (unsigned __int128)45;

    /* part = (n + 4) * t */
    unsigned __int128 part = (tn + (unsigned __int128)4) * t;

    /* binom = binomial(n+7, 7) computed multiplicatively */
    unsigned __int128 bin = 1;
    idx_t N = n + 7;
    for (idx_t i = 1; i <= 7; ++i)
    {
        /* bin = bin * (n + i) / i  */
        bin = bin * (tn + (unsigned __int128)i) / (unsigned __int128)i;
    }

    /* result = part * bin / 180  (180 = 4*45) */
    unsigned __int128 res = part * bin / (unsigned __int128)180;

    return (seq_t)res;
}
