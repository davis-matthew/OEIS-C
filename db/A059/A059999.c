/*
 * A059999  a(n) = (1/6)*n^5 - (19/8)*n^4 + (51/4)*n^3 - (253/8)*n^2 +
 *          (445/12)*n - 14.
 * https://oeis.org/A059999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dumb, easy, nonn
 * Formula:   a(n) = (4*n^5 - 57*n^4 + 306*n^3 - 759*n^2 + 890*n - 336)/24
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 123, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset return an arbitrary value */
    if (n < 1) return 0ull;

    /* Use a 128‑bit signed integer for intermediate arithmetic.  GCC/Clang
       provide the `__int128` type as a GNU extension that is available in
       both C11 (when compiling as an extension) and in C++17.  */
    signed __int128 N = (signed __int128)n;

    signed __int128 term =
        (signed __int128)4  * N * N * N * N * N
      - (signed __int128)57 * N * N * N * N
      + (signed __int128)306 * N * N * N
      - (signed __int128)759 * N * N
      + (signed __int128)890 * N
      - (signed __int128)336;

    term /= 24;                     /* division yields an integer */

    return (seq_t)term;
}
