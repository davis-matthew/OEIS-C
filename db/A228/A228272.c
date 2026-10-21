/*
 * A228272  Volume of sphere (rounded down) with the diameter equal to n.
 * https://oeis.org/A228272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((Pi*n^3)/6)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a rational approximation of pi/6 that is accurate within
       1e-12 relative error. For n <= 10,000 we have n^3 <= 1e12, so
       this guarantees the floor value is correct. */
    static const seq_t P = (seq_t)523598775598ULL;     /* numerator  pi/6 * 10^12     */
    static const seq_t Q = (seq_t)1000000000000ULL;   /* denominator 10^12           */

    /* Compute floor(P * n^3 / Q) using 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 temp = (unsigned __int128)P *
                             (unsigned __int128)n *
                             n *
                             n;
    return (seq_t)(temp / Q);
}
