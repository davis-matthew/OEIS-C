/*
 * A060999  Nearest integer to (n+1)^3/9.
 * https://oeis.org/A060999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to ((n+1)^3)/9
 * Range:     u8 n <= 12, u16 n <= 82, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow when seq_t is small. */
    unsigned __int128 m = (unsigned __int128)(n + 1);
    unsigned __int128 t = m * m * m;          /* (n+1)^3 */
    unsigned __int128 q = t / 9;              /* integer part */
    unsigned __int128 r = t % 9;              /* remainder */

    /* round to nearest integer: add 1 if remainder*2 >= 9 */
    if (r * 2 >= 9)
        q += 1;

    return (seq_t)q;
}
