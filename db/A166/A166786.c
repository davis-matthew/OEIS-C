/*
 * A166786  Number of nX4 1..2 arrays containing at least one of each value,
 *          all equal values connected, and rows considered as a single number
 *          in nondecreasing order.
 * https://oeis.org/A166786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (2*n^5 + 15*n^4 + 60*n^3 - 75*n^2 + 268*n - 90)/30
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use unsigned __int128 to avoid overflow for 64‑bit seq_t builds */
    unsigned __int128 np = n;
    unsigned __int128 n2 = np * np;          /* n^2 */
    unsigned __int128 n3 = n2 * np;          /* n^3 */
    unsigned __int128 n4 = n3 * np;          /* n^4 */
    unsigned __int128 n5 = n4 * np;          /* n^5 */
    unsigned __int128 num = 2*n5 + 15*n4 + 60*n3 - 75*n2 + 268*np - 90;
    return (seq_t)(num / 30);
}
