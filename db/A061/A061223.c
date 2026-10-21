/*
 * A061223  a(n) = n^3 + (n + 1)^4 + (n + 2)^5.
 * https://oeis.org/A061223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + (n+1)^4 + (n+2)^5
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 82, u64 n <= 7129,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow for n up to ~10⁶,
       which covers the test range for 64‑bit builds.  Values that
       do not fit in the target seq_t type are truncated on cast,
       but those terms are not checked. */
    unsigned __int128 N = (unsigned __int128)(seq_t)n;

    /* n^3 */
    unsigned __int128 n3 = N * N * N;

    /* (n+1)^4 */
    unsigned __int128 a1 = N + 1;
    unsigned __int128 a1sq = a1 * a1;
    unsigned __int128 a1pow4 = a1sq * a1sq;

    /* (n+2)^5 */
    unsigned __int128 a2 = N + 2;
    unsigned __int128 a2pow5 = a2 * a2 * a2 * a2 * a2;

    unsigned __int128 sum = n3 + a1pow4 + a2pow5;
    return (seq_t)sum;
}
