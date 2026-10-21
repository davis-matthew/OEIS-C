/*
 * A210430  Number of semistandard Young tableaux over all partitions of 8
 *          with maximal element <= n.
 * https://oeis.org/A210430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2+3)*(1124+(1205+191*n^2)*n^2)/10080
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 26, u64 n <= 420,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow for 64‑bit builds where
       the final value still fits.  The intermediate result may exceed
       64 bits for large n, but that is allowed because seq_t is either
       an arbitrary‑precision type or a type large enough to hold the
       result. */
    unsigned __int128 un = (unsigned __int128)n;
    unsigned __int128 n2 = un * un;                     /* n^2          */
    unsigned __int128 term3 = 1124 + (1205 + 191 * n2) * n2;
    unsigned __int128 num   = n2 * (n2 + 3) * term3;      /* numerator   */
    unsigned __int128 res   = num / (unsigned __int128)10080;
    return (seq_t)res;
}
