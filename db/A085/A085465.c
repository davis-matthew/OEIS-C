/*
 * A085465  Number of monotone n-weightings of complete bipartite digraph
 *          K(3,3).
 * https://oeis.org/A085465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/20)*n*(n+1)*(n^2+1)*(n^2+2*n+2)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 65, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset of the sequence is 1.  For n < 1 the value is undefined, but
       we return 0 for safety. */
    if (n == 0)
        return 0;

    /* Compute the expression using 128‑bit intermediate to avoid overflow. */
    __int128 N = n;

    __int128 prod = N * (N + 1);                /* n*(n+1)         */
    prod *= (N * N + 1);                       /* *(n^2+1)         */
    prod *= (N * N + 2 * N + 2);               /* *(n^2+2n+2)      */
    prod /= 20;                                 /* divide by 20     */

    return (seq_t)prod;
}
