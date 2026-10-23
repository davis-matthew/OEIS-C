/*
 * A116803  Number of permutations of length n which avoid the patterns 1234,
 *          3421, 4231.
 * https://oeis.org/A116803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^6 + 9*n^5 - 205*n^4 + 315*n^3 + 3080*n^2 - 10404*n +
 *            10080)/720 for n ≥ 2; a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset.  The definition is valid for n ≥ 1. */
    if (n <= 1) {
        return (seq_t)1;
    }

    /* Use signed 128‑bit arithmetic to avoid overflow while computing
       the polynomial.  This works for any size of seq_t (uint8_t…__int128
       or arbitrary‑precision).  The result fits in seq_t for all n where
       the sequence value does. */
    __int128 N = n;               /* promote to 128‑bit signed */
    __int128 n2 = N * N;          /* n² */
    __int128 n3 = n2 * N;         /* n³ */
    __int128 n4 = n3 * N;         /* n⁴ */
    __int128 n5 = n4 * N;         /* n⁵ */
    __int128 n6 = n5 * N;         /* n⁶ */

    __int128 num =   ( (__int128)5  * n6 )
                    + ( (__int128)9  * n5 )
                    + ( (__int128)-205 * n4 )
                    + ( (__int128)315 * n3 )
                    + ( (__int128)3080 * n2 )
                    + ( (__int128)-10404 * N )
                    + ( (__int128)10080 );

    /* Division is exact for n ≥ 2. */
    return (seq_t)(num / 720);
}
