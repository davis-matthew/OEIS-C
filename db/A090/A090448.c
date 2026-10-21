/*
 * A090448  Fourth column (m=3) of triangle A090447.
 * https://oeis.org/A090448
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6-4*n^5+5*n^4-2*n^3)/12, for n>=3
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* use 128‑bit arithmetic for the intermediate polynomial */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;        /* n^2      */
    unsigned __int128 n3 = n2 * nn;        /* n^3      */
    unsigned __int128 n4 = n3 * nn;        /* n^4      */
    unsigned __int128 n5 = n4 * nn;        /* n^5      */
    unsigned __int128 n6 = n5 * nn;        /* n^6      */

    unsigned __int128 val = n6 - 4 * n5 + 5 * n4 - 2 * n3;
    val /= 12;                              /* integer division is exact */

    return (seq_t)val;
}
