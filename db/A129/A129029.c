/*
 * A129029  a(n) = 8*n^4+44*n^3+106*n^2+100*n+30.
 * https://oeis.org/A129029
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^4+44*n^3+106*n^2+100*n+30
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset where a(-1) = 0 */
    if (n < 0)
        return (seq_t)0;

    /* Perform the calculation in a signed 128‑bit integer to avoid
       overflow for reasonably large n.  The result is then cast to seq_t,
       which will truncate correctly for any unsigned type. */
    __int128 x  = (__int128)n;
    __int128 x2 = x * x;
    __int128 x3 = x2 * x;
    __int128 x4 = x3 * x;

    __int128 v = 8 * x4 + 44 * x3 + 106 * x2 + 100 * x + 30;
    return (seq_t)v;
}
