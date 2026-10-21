/*
 * A210429  Number of semistandard Young tableaux over all partitions of 7
 *          with maximal element <= n.
 * https://oeis.org/A210429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(180+(637+(385+58*n^2)*n^2)*n^2)/1260
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 36, u64 n <= 877, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use an unsigned 128‑bit integer for safe intermediate calculations.
       All terms are non‑negative, so unsigned arithmetic is sufficient. */
    unsigned __int128 m = n;          /* Convert n to 128‑bit. */
    unsigned __int128 p = (unsigned __int128)58;

    /* Horner-like evaluation of the polynomial
       p = 58*m^6 + 385*m^4 + 637*m^2 + 180. */
    p = p * m * m + (unsigned __int128)385;
    p = p * m * m + (unsigned __int128)637;
    p = p * m * m + (unsigned __int128)180;

    /* Multiply by m to get the full numerator:
       num = 58*m^7 + 385*m^5 + 637*m^3 + 180*m. */
    unsigned __int128 num = p * m;

    /* Divide by 1260, the denominator of the formula. */
    unsigned __int128 res = num / (unsigned __int128)1260;

    /* Convert back to the required seq_t type. */
    return (seq_t)res;
}
