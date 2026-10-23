/*
 * A107965  a(n) = (n+1)(n+2)^2*(n+3)^2*(n+4)(11n^4 + 110n^3 + 439n^2 + 820n +
 *          600)/86400.
 * https://oeis.org/A107965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)(n+2)^2(n+3)^2(n+4)(11n^4 + 110n^3 + 439n^2 + 820n +
 *            600)/86400
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 20, u64 n <= 204, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 prod = 1;

    prod *= ((unsigned __int128)n + 1);
    prod *= ((unsigned __int128)n + 2);
    prod *= ((unsigned __int128)n + 2);
    prod *= ((unsigned __int128)n + 3);
    prod *= ((unsigned __int128)n + 3);
    prod *= ((unsigned __int128)n + 4);

    unsigned __int128 nn   = n;
    unsigned __int128 poly = 11 * nn * nn * nn * nn
                          + 110 * nn * nn * nn
                          + 439 * nn * nn
                          + 820 * nn
                          + 600;
    prod *= poly;

    prod /= (unsigned __int128)86400;

    return (seq_t)prod;
}
