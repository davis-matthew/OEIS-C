/*
 * A223773  Number of n X 4 0..1 arrays with rows and columns unimodal and
 *          antidiagonals nondecreasing.
 * https://oeis.org/A223773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 11; for n>1, a(n) = ( n^8 + 12 n^7 + 266 n^6 + 2352 n^5 +
 *            17129 n^4 + 65268 n^3 + 506764 n^2 - 591792 n + 524160 ) / 40320
 *            (derived from the empirical polynomial in the OEIS entry)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 58, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)11;

    /* use 128‑bit arithmetic for safety of intermediate values */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 p2 = N * N;                /* n^2 */
    unsigned __int128 p3 = p2 * N;               /* n^3 */
    unsigned __int128 p4 = p3 * N;               /* n^4 */
    unsigned __int128 p5 = p4 * N;               /* n^5 */
    unsigned __int128 p6 = p5 * N;               /* n^6 */
    unsigned __int128 p7 = p6 * N;               /* n^7 */
    unsigned __int128 p8 = p7 * N;               /* n^8 */

    unsigned __int128 pos = p8
                           + (unsigned __int128)12 * p7
                           + (unsigned __int128)266  * p6
                           + (unsigned __int128)2352 * p5
                           + (unsigned __int128)17129* p4
                           + (unsigned __int128)65268* p3
                           + (unsigned __int128)506764* p2
                           + (unsigned __int128)524160;

    unsigned __int128 num = pos
                           - (unsigned __int128)591792 * N;

    return (seq_t)(num / (unsigned __int128)40320);
}
