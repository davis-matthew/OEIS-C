/*
 * A219090  a(n) = floor((n + 1/2)^7).
 * https://oeis.org/A219090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1/2)^7) = ((2*n + 1)^7) / 128
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 564,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid intermediate overflow. */
    unsigned __int128 p   = (unsigned __int128)(2 * (unsigned __int128)n + 1);
    unsigned __int128 pow = p;          /* p^1 */
    pow *= p;                            /* p^2 */
    pow *= p;                            /* p^3 */
    pow *= p;                            /* p^4 */
    pow *= p;                            /* p^5 */
    pow *= p;                            /* p^6 */
    pow *= p;                            /* p^7 */
    pow >>= 7;                           /* divide by 128 */
    return (seq_t)pow;
}
