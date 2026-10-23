/*
 * A153445  Take n points in general position in space; draw all planes
 *          through all triples of these points; sequence gives number of
 *          connected regions formed.
 * https://oeis.org/A153445
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n^9 - 9n^8 + 6n^7 + 180n^6 - 663n^5 + 369n^4 +
 *            3536n^3 - 8964n^2 + 2952n) / 1296 (valid for n ≥ 3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 3)
        return (seq_t)2;

    /* Use 128‑bit arithmetic for all intermediates to avoid overflow.
       The result is cast back to seq_t; it will fit when the sequence
       value fits in the type. */
    unsigned __int128 N = (unsigned __int128)n;

    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;
    unsigned __int128 n5 = n4 * N;
    unsigned __int128 n6 = n5 * N;
    unsigned __int128 n7 = n6 * N;
    unsigned __int128 n8 = n7 * N;
    unsigned __int128 n9 = n8 * N;

    unsigned __int128 num = n9
        - (__int128)9   * n8
        + (__int128)6   * n7
        + (__int128)180 * n6
        - (__int128)663 * n5
        + (__int128)369 * n4
        + (__int128)3536 * n3
        - (__int128)8964 * n2
        + (__int128)2952 * N;

    unsigned __int128 res = 1 + num / 1296;

    return (seq_t)res;
}
