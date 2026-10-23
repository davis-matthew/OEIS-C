/*
 * A008414  Coordination sequence for 6-dimensional cubic lattice.
 * https://oeis.org/A008414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(2/15*n^4+4/3*n^2+23/15) a(0) = 1 (uses 128‑bit
 *            arithmetic for the intermediate product)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 95, u64 n <= 8086,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* cast to a wider type for safe intermediate arithmetic */
    __int128 N  = (__int128)n;
    __int128 n2 = N * N;        /* n^2 */
    __int128 n3 = n2 * N;       /* n^3 */
    __int128 n5 = n3 * n2;      /* n^5 */

    /* numerator: 8*n^5 + 80*n^3 + 92*n */
    __int128 num = (__int128)8 * n5
                 + (__int128)80 * n3
                 + (__int128)92 * N;

    /* exact division by 15 */
    seq_t result = (seq_t)(num / 15);
    return result;
}
