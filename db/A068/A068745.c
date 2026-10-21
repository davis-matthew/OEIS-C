/*
 * A068745  Number of potential flows in 4 X 4 array with integer velocities
 *          in -n..n, i.e., number of 4 X 4 arrays with adjacent elements
 *          differing by no more than n, counting arrays differing by a
 *          constant only once.
 * https://oeis.org/A068745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = y*(2623243666*y^14 + 9598591135*y^12 + 17180805187*y^10 +
 *            20342655905*y^8 + * 17636121503*y^6 + 10907793260*y^4 +
 *            3135618144*y^2 + 304819200) / 81729648000, * where y = 2*n + 1
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use unsigned __int128 for all intermediate arithmetic.  The division
       is exact for all n; for n where the result does not fit into seq_t
       the wrap‑around of unsigned arithmetic is acceptable. */
    unsigned __int128 y   = (unsigned __int128)n * 2 + 1;
    unsigned __int128 y2  = y * y;
    unsigned __int128 y4  = y2 * y2;
    unsigned __int128 y6  = y4 * y2;
    unsigned __int128 y8  = y4 * y4;
    unsigned __int128 y10 = y8 * y2;
    unsigned __int128 y12 = y6 * y6;
    unsigned __int128 y14 = y12 * y2;

    unsigned __int128 poly =
        (unsigned __int128)2623243666ULL * y14 +
        (unsigned __int128)9598591135ULL * y12 +
        (unsigned __int128)17180805187ULL * y10 +
        (unsigned __int128)20342655905ULL * y8  +
        (unsigned __int128)17636121503ULL * y6  +
        (unsigned __int128)10907793260ULL * y4  +
        (unsigned __int128)3135618144ULL * y2  +
        (unsigned __int128)304819200ULL;

    unsigned __int128 num = y * poly;
    const unsigned __int128 den = (unsigned __int128)81729648000ULL;
    unsigned __int128 res = num / den;

    return (seq_t)res;
}
