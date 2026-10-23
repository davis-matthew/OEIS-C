/*
 * A028295  a(n) = n^6 - (883/60)*n^5 + (157/3)*n^4 + (2155/12)*n^3 -
 *          (4570/3)*n^2 + (42767/15)*n - 967.
 * https://oeis.org/A028295
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 - (883/60)*n^5 + (157/3)*n^4 + (2155/12)*n^3 -
 *            (4570/3)*n^2 + (42767/15)*n - 967, a(6)=133
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;                 /* offset handling – value is undefined below 6 */

    /* Compute 60*a(n) using Horner's rule to avoid overflow in 64‑bit builds.
       The intermediate fits comfortably in signed 128‑bit for n up to at least 10000. */
    __int128 r = 60;                 /* start with leading coefficient */
    r = r * (__int128)n - 883;       /* -883 * n^5 term */
    r = r * (__int128)n + 3140;      /* +3140 * n^4   */
    r = r * (__int128)n + 10775;     /* +10775 * n^3  */
    r = r * (__int128)n - 91400;     /* -91400 * n^2  */
    r = r * (__int128)n + 171068;    /* +171068 * n   */
    r = r * (__int128)n - 58020;     /* -58020 constant */
    r /= 60;                         /* divide by LCM of denominators */

    return (seq_t)r;
}
