/*
 * A166758  Number of nX2 1..5 arrays containing at least one of each value,
 *          and all equal values connected.
 * https://oeis.org/A166758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/21)*(n-1)*(n-2)*(16*n^6 - 80*n^5 + 512*n^4 - 1720*n^3
 *            + 3525*n^2 - 4227*n + 2520)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    seq_t nn = n;
    unsigned __int128 un = (unsigned __int128)nn;

    unsigned __int128 n2 = un * un;          /* n^2 */
    unsigned __int128 n3 = n2 * un;          /* n^3 */
    unsigned __int128 n4 = n2 * n2;          /* n^4 */
    unsigned __int128 n5 = n4 * un;          /* n^5 */
    unsigned __int128 n6 = n4 * n2;          /* n^6 */

    unsigned __int128 poly =
            16u * n6
          - 80u * n5
          + 512u * n4
          - 1720u * n3
          + 3525u * n2
          - 4227u * un
          + 2520u;

    unsigned __int128 prod = poly *
                             (unsigned __int128)(n - 1) *
                             (unsigned __int128)(n - 2);
    prod /= 21u;

    return (seq_t)prod;
}
