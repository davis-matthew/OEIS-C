/*
 * A093566  a(n) = n*(n-1)*(n-2)*(n-3)*(n^2-3*n-2)/48.
 * https://oeis.org/A093566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(n*n-3*n-2)/48
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) = 0 for n < 4 */
    if (n < 4)
        return 0;

    /* Use a 128‑bit intermediate to avoid overflow on the 64‑bit build.
       The result always fits in seq_t for the tested range. */
    unsigned __int128 prod = 1;

    prod *= (unsigned __int128)n;
    prod *= (unsigned __int128)(n - 1);
    prod *= (unsigned __int128)(n - 2);
    prod *= (unsigned __int128)(n - 3);

    /* Compute the quadratic term in 128‑bit to keep it exact. */
    unsigned __int128 quad = (unsigned __int128)n * (unsigned __int128)n; /* n² */
    quad -= (unsigned __int128)3 * n;                                   /* -3n */
    quad -= 2;                                                           /* -2 */

    prod *= quad;
    prod /= 48u;  /* final division by 48 */

    return (seq_t)prod;
}
