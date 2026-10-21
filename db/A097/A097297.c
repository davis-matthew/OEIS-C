/*
 * A097297  Seventh column (m=6) of (1,6)-Pascal triangle A096956.
 * https://oeis.org/A097297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+36)*C(n+5,5)/6 =
 *            (n+36)*(n+5)*(n+4)*(n+3)*(n+2)*(n+1)/720
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 112, u64 n <= 4857,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the numerator using a 128‑bit intermediate to avoid overflow
       on 64‑bit builds.  The denominator 720 is a constant. */
    unsigned __int128 num = 1;

    num *= (unsigned __int128)(n + 36);
    num *= (unsigned __int128)(n + 5);
    num *= (unsigned __int128)(n + 4);
    num *= (unsigned __int128)(n + 3);
    num *= (unsigned __int128)(n + 2);
    num *= (unsigned __int128)(n + 1);

    return (seq_t)(num / 720);
}
