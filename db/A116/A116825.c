/*
 * A116825  Number of permutations of length n which avoid the patterns 1243,
 *          2341, 4321.
 * https://oeis.org/A116825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^4 - 16*n^3 + 43*n^2 - 752*n + 2388)/12 for n ≥ 6
 *            a(1) = 1, a(2) = 2, a(3) = 6, a(4) = 21, a(5) = 71
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 319, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values for n = 1..5 */
    static const seq_t init[] = {1, 2, 6, 21, 71};

    if (n >= 1 && n <= 5)
        return init[n - 1];

    unsigned __int128 N = n;
    unsigned __int128 v = 5 * N * N * N * N;
    v -= 16 * N * N * N;
    v += 43 * N * N;
    v -= 752 * N;
    v += 2388;
    v /= 12;

    return (seq_t)v;
}
