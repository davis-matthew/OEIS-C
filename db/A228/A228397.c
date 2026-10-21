/*
 * A228397  The number of permutations of length n sortable by 3 reversals.
 * https://oeis.org/A228397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 318 + n*(7*n^5 - 21*n^4 - 125*n^3 - 819*n^2 + 12862*n -
 *            42720)/144 for n > 3; a(1) = 1, a(2) = 2, a(3) = 6
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small values -> factorial (up to n = 3 are the only cases needed) */
    if (n <= 3) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)6;
            default: return (seq_t)0; /* unreachable */
        }
    }

    /* Use signed 128‑bit intermediate arithmetic; it is far larger than needed
       for all practical n (fits n up to at least 10^6 comfortably). */
    __int128 N  = (__int128)n;
    __int128 n2 = N * N;
    __int128 n3 = n2 * N;
    __int128 n4 = n3 * N;
    __int128 n5 = n4 * N;
    __int128 n6 = n5 * N;

    /* Compute the polynomial 7*n^6 - 21*n^5 - 125*n^4 - 819*n^3 + 12862*n^2 - 42720*n */
    __int128 poly = 7 * n6
                  - 21 * n5
                  - 125 * n4
                  - 819 * n3
                  + 12862 * n2
                  - 42720 * N;

    /* Final formula: 318 + poly/144 */
    __int128 result = ( __int128)318 + poly / 144;

    return (seq_t)result;
}
