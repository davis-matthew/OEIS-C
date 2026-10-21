/*
 * A076462  Sum of squares of numbers that can be written as t*n + u*(n+1) for
 *          nonnegative integers t,u in exactly three ways.
 * https://oeis.org/A076462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(55*n^4+74*n^3+6*n^2-13*n+1)/6
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 27, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* The sequence is 1‑based */

    /* 128‑bit intermediates are sufficient for n up to 10 000 and all
       64‑bit built‑ins. They safely hold the product before the final
       division by 6. */
    unsigned __int128 N  = n;          /* n                     */
    unsigned __int128 N1 = N + 1;      /* n+1                   */
    unsigned __int128 N2 = N * N;      /* n^2                   */
    unsigned __int128 N3 = N2 * N;     /* n^3                   */
    unsigned __int128 N4 = N3 * N;     /* n^4                   */

    /* Polynomial part: 55*n^4 + 74*n^3 + 6*n^2 – 13*n + 1 */
    unsigned __int128 poly = (unsigned __int128)55 * N4
                           + (unsigned __int128)74 * N3
                           + (unsigned __int128)6  * N2
                           - (unsigned __int128)13 * N
                           + (unsigned __int128)1;

    /* Compute the product n*(n+1)*poly and divide by 6 */
    unsigned __int128 prod = N * N1;   /* n*(n+1)      */
    prod *= poly;                     /* n*(n+1)*poly */
    prod /= 6;                        /* final value  */

    return (seq_t)prod;
}
