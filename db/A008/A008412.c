/*
 * A008412  Coordination sequence for 4-dimensional cubic lattice (points on
 *          surface of 4-dimensional cross-polytope).
 * https://oeis.org/A008412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 for n=0, otherwise 8*n*(n^2+2)/3
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 1172, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    unsigned __int128 n1 = (unsigned __int128)n;
    unsigned __int128 n2 = n1 * n1 + 2;

    /* divide by 3 using the factor that is guaranteed to be divisible */
    if (n % 3 == 0)
        n1 /= 3;
    else
        n2 /= 3;

    unsigned __int128 val = 8 * n1 * n2;
    return (seq_t)val;
}
