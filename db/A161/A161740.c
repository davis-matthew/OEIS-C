/*
 * A161740  Fourth right hand column of the RSEG2 triangle A161739.
 * https://oeis.org/A161740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            (n+4)*(n+3)*(n+2)*(n+1)*(n)*(n-1)*(35*n^3+189*n^2+325*n+204)/45360
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 just in case. */
    if (n < 2) return 0;

    /* Use a 128‑bit intermediate to avoid overflow for the index
       range of interest.  All intermediate results stay below 2^128. */
    unsigned __int128 nn = (unsigned __int128)n;

    /* First factor: product of the six consecutive integers. */
    unsigned __int128 prod =
        (nn - 1) * nn * (nn + 1) * (nn + 2) * (nn + 3) * (nn + 4);

    /* Polynomial part: 35*n^3 + 189*n^2 + 325*n + 204 */
    unsigned __int128 poly =
        (unsigned __int128)35 * nn * nn * nn
      + (unsigned __int128)189 * nn * nn
      + (unsigned __int128)325 * nn
      + (unsigned __int128)204;

    /* Multiply the two factors and divide by the constant denominator. */
    prod *= poly;
    prod /= 45360;

    return (seq_t)prod;
}
