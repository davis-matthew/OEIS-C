/*
 * A219933  Number of nX2 arrays of the minimum value of corresponding
 *          elements and their horizontal or diagonal neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..3 nX2 array.
 * https://oeis.org/A219933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^8 + 12 n^7 + 14 n^6 + 1932 n^5 - 4291 n^4 + 35448 n^3
 *            + 44596 n^2 + 2928 n) / 20160 for n >= 1 (OEIS offset 1).
 *            Integer arithmetic using 128‑bit intermediate to avoid overflow
 *            on n up to ~10 000
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 53, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* below offset; unused */
    /* use a 128‑bit signed type for safe intermediate calculations */
    __int128 nn = (unsigned __int128)n;     /* promote n to unsigned 128‑bit first */
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 n4 = n3 * nn;
    __int128 n5 = n4 * nn;
    __int128 n6 = n5 * nn;
    __int128 n7 = n6 * nn;
    __int128 n8 = n7 * nn;

    __int128 p = n8
               + 12 * n7
               + 14 * n6
               + 1932 * n5
               - 4291 * n4
               + 35448 * n3
               + 44596 * n2
               + 2928 * nn;

    return (seq_t)(p / 20160);
}
