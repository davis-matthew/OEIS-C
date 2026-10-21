/*
 * A062127  Seventh column of A046741.
 * https://oeis.org/A062127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (81*n^6 + 567*n^5 + 2205*n^4 + 4545*n^3 + 5674*n^2 +
 *            3728*n + 1040)/80
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 x = n;
    unsigned __int128 n2 = x * x;          /* n^2 */
    unsigned __int128 n3 = n2 * x;         /* n^3 */
    unsigned __int128 n4 = n3 * x;         /* n^4 */
    unsigned __int128 n5 = n4 * x;         /* n^5 */
    unsigned __int128 n6 = n5 * x;         /* n^6 */

    unsigned __int128 sum =
        (unsigned __int128)81   * n6 +
        (unsigned __int128)567  * n5 +
        (unsigned __int128)2205 * n4 +
        (unsigned __int128)4545 * n3 +
        (unsigned __int128)5674 * n2 +
        (unsigned __int128)3728 * x  +
        (unsigned __int128)1040;

    return (seq_t)(sum / 80);
}
