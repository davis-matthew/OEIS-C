/*
 * A225011  Number of 4 X n 0..1 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A225011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n+1)*(n^6 + 27*n^5 + 351*n^4 + 2449*n^3 + 10760*n^2 +
 *            25052*n + 42000))/40320 + 1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 56, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but safeguard */
    unsigned __int128 x  = n;
    unsigned __int128 x2 = x * x;
    unsigned __int128 x3 = x2 * x;
    unsigned __int128 x4 = x3 * x;
    unsigned __int128 x5 = x4 * x;
    unsigned __int128 x6 = x5 * x;

    unsigned __int128 p =
          x6
        + (unsigned __int128)27 * x5
        + (unsigned __int128)351 * x4
        + (unsigned __int128)2449 * x3
        + (unsigned __int128)10760 * x2
        + (unsigned __int128)25052 * x
        + (unsigned __int128)42000;

    unsigned __int128 tmp = x * (x + 1);
    unsigned __int128 numerator = tmp * p;
    unsigned __int128 res = numerator / 40320 + 1;
    return (seq_t)res;
}
