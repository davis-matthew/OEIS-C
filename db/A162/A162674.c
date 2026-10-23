/*
 * A162674  Number of different fixed (possibly) disconnected tetrominoes
 *          bounded (not necessarily tightly) by an n X n square.
 * https://oeis.org/A162674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(8*n^4-16*n^3-9*n^2+17*n+8)/12
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1 – for n < 1 return 0 (unnecessary for valid input) */
    if (n < 1) return 0;

    /* Use a type that can hold the intermediate values for all
       practical n (up to about 10 000).  unsigned __int128 is
       available on GCC/Clang and is larger than any seq_t in the
       builds that are expected. */
    unsigned __int128 sn  = (unsigned __int128)n;
    unsigned __int128 poly =
        (unsigned __int128)8 * sn * sn * sn * sn        /* 8*n⁴ */
      - (unsigned __int128)16 * sn * sn * sn            /* 16*n³ */
      - (unsigned __int128)9  * sn * sn                 /* 9*n²  */
      + (unsigned __int128)17 * sn                      /* 17*n  */
      + 8;                                             /* +8   */

    unsigned __int128 res = sn * (sn - 1) * poly / 12;

    return (seq_t)res;   /* cast down to the required seq_t type */
}
