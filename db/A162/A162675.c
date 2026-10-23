/*
 * A162675  Number of different fixed (possibly) disconnected pentominoes
 *          bounded (not necessarily tightly) by an n*n square.
 * https://oeis.org/A162675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n+1)*(5*n^4-10*n^3-7*n^2+12*n+6)/24
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 20, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;             /* terms for n=1,2 are zero */

    unsigned __int128 N = (unsigned __int128)n;

    unsigned __int128 a_prod = N * (N - 1) * (N - 2) * (N + 1);

    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;

    unsigned __int128 b = 5 * n4 - 10 * n3 - 7 * n2 + 12 * N + 6;

    unsigned __int128 prod = a_prod * b;
    prod /= 24;

    return (seq_t)prod;
}
