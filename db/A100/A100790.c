/*
 * A100790  First differences of A047780.
 * https://oeis.org/A100790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/24)(n+1)(6n^4 + 9n^3 + 23n^2 + 46n + 24)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider integer type to avoid overflow in intermediate
       calculations when seq_t is 64‑bit or smaller.  */
    unsigned __int128 sn = (unsigned __int128)n;          // n
    unsigned __int128 n1 = sn + (unsigned __int128)1;    // n+1
    unsigned __int128 n2 = sn * sn;                      // n^2
    unsigned __int128 n3 = n2 * sn;                      // n^3
    unsigned __int128 n4 = n3 * sn;                      // n^4

    unsigned __int128 poly = (unsigned __int128)6 * n4
                            + (unsigned __int128)9 * n3
                            + (unsigned __int128)23 * n2
                            + (unsigned __int128)46 * sn
                            + (unsigned __int128)24;

    unsigned __int128 result = n1 * poly / (unsigned __int128)24;
    return (seq_t)result;
}
