/*
 * A085437  Duplicate of A024166.
 * https://oeis.org/A085437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = Σ_{k=1..n} (k(k+1)/2)^2 = n(n+1)(3n^3+12n^2+13n+2)/60
 *            a(0) = 0
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* use a larger integer type to avoid overflow on 64‑bit builds.
       __int128 is available on the platforms used for these builds. */
    __int128 N = ( __int128 )n;
    __int128 term = 3 * N * N * N + 12 * N * N + 13 * N + 2;
    __int128 prod = N * (N + 1) * term;    /* numerator */
    prod /= 60;                            /* divide by 60 */
    return (seq_t)prod;
}
