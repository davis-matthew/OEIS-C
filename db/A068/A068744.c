/*
 * A068744  Number of potential flows in 3 X 3 array with integer velocities
 *          in -n..n, i.e., number of 3 X 3 arrays with adjacent elements
 *          differing by no more than n, counting arrays differing by a
 *          constant only once.
 * https://oeis.org/A068744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( (2*n+1)^2 * (529*(2*n+1)^6 + 910*(2*n+1)^4 * +
 *            721*(2*n+1)^2 + 360) ) / 2520 * (where n ≥ 0)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence can be evaluated via a closed‑form
       polynomial in y = 2*n + 1.
       For seq_t fitting in 128 bits, use an unsigned __int128
       intermediate to avoid overflow; otherwise rely on the
       arbitrary‑precision arithmetic of seq_t. */

    if (sizeof(seq_t) <= sizeof(unsigned __int128)) {
        unsigned long long n64 = (unsigned long long)n; /* still fits in 64 bits */
        unsigned __int128 y   = (unsigned __int128)2 * n64 + 1;      /* y = 2*n+1 */
        unsigned __int128 y2  = y * y;
        unsigned __int128 y4  = y2 * y2;
        unsigned __int128 y6  = y4 * y2;

        unsigned __int128 inner = (unsigned __int128)529 * y6
                               + (unsigned __int128)910 * y4
                               + (unsigned __int128)721 * y2
                               + (unsigned __int128)360;

        unsigned __int128 num   = y2 * inner;
        unsigned __int128 res   = num / (unsigned __int128)2520;

        return (seq_t)res;
    } else {
        seq_t y   = (seq_t)2 * (seq_t)n + (seq_t)1;   /* y = 2*n+1 */
        seq_t y2  = y * y;
        seq_t y4  = y2 * y2;
        seq_t y6  = y4 * y2;

        seq_t inner = (seq_t)529 * y6
                    + (seq_t)910 * y4
                    + (seq_t)721 * y2
                    + (seq_t)360;

        seq_t num   = y2 * inner;
        seq_t res   = num / (seq_t)2520;

        return res;
    }
}
