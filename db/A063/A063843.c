/*
 * A063843  Number of n-multigraphs on 5 nodes.
 * https://oeis.org/A063843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (24*n^2 + 50*n^3 + 20*n^4 + 15*n^6 + 10*n^7 + n^10)/120
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 136, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses 128‑bit arithmetic to avoid overflow on 64‑bit builds
   and works with arbitrary‑precision integer types as well. */

seq_t a(idx_t n)
{
    /* Convert the index to a 128‑bit intermediate to avoid overflow. */
    unsigned __int128 sn = n;

    /* Compute successive powers of n. */
    unsigned __int128 n2 = sn * sn;            /* n^2 */
    unsigned __int128 n3 = n2 * sn;            /* n^3 */
    unsigned __int128 n4 = n3 * sn;            /* n^4 */
    unsigned __int128 n5 = n4 * sn;            /* n^5 */
    unsigned __int128 n6 = n3 * n3;            /* n^6 */
    unsigned __int128 n7 = n6 * sn;            /* n^7 */
    unsigned __int128 n10 = n5 * n5;           /* n^10 */

    /* Evaluate the numerator of the formula. */
    unsigned __int128 expr =
        (unsigned __int128)24 * n2 +
        (unsigned __int128)50 * n3 +
        (unsigned __int128)20 * n4 +
        (unsigned __int128)15 * n6 +
        (unsigned __int128)10 * n7 +
        n10;

    /* Final division yields an integer result. */
    unsigned __int128 result = expr / 120;

    return (seq_t)result;
}
