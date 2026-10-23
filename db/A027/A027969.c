/*
 * A027969  a(n) = T(n, 2*n-7), T given by A027960.
 * https://oeis.org/A027969
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (90720 - 85548*n + 38822*n^2 - 10136*n^3 + 1505*n^4 -
 *            77*n^5 - 7*n^6 + n^7)/5040 (n ≥ 5), a(4) = 3
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 81, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 4.  For n = 4 the
       polynomial fails, so handle it separately. */
    if (n == 4)
        return (seq_t)3;

    /* Use an unsigned 128‑bit type for the intermediate
       arithmetic so that the computation is exact for all
       values that fit in seq_t.  For seq_t implementations
       that are 64‑bit or less the result still fits because
       the division by 5040 guarantees it.  */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n1 = N;
    unsigned __int128 n2 = n1 * n1;        /* n^2 */
    unsigned __int128 n3 = n2 * n1;        /* n^3 */
    unsigned __int128 n4 = n3 * n1;        /* n^4 */
    unsigned __int128 n5 = n4 * n1;        /* n^5 */
    unsigned __int128 n6 = n5 * n1;        /* n^6 */
    unsigned __int128 n7 = n6 * n1;        /* n^7 */

    unsigned __int128 num =
          (unsigned __int128)90720
        - (unsigned __int128)85548 * n1
        + (unsigned __int128)38822 * n2
        - (unsigned __int128)10136 * n3
        + (unsigned __int128)1505  * n4
        - (unsigned __int128)77    * n5
        - (unsigned __int128)7     * n6
        + n7;

    seq_t result = (seq_t)(num / (unsigned __int128)5040);
    return result;
}
