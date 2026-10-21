/*
 * A061003  Nearest integer to n^5/25.
 * https://oeis.org/A061003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^5 + 12) / 25)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 160, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit temporary to avoid overflow during n^5.
       The result fits in seq_t for all n up to at least 1e9. */
    unsigned __int128 v = (unsigned __int128)n;
    v *= n;           /* n^2 */
    v *= n;           /* n^3 */
    v *= n;           /* n^4 */
    v *= n;           /* n^5 */
    v += 12;          /* add 12 for rounding to nearest integer */
    v /= 25;          /* divide by 25 */
    return (seq_t)v;  /* cast to the build’s seq_t type */
}
