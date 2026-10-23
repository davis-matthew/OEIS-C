/*
 * A160829  Expansion of (1 + 44*x + 337*x^2 + 612*x^3 + 305*x^4 + 40*x^5 +
 *          x^6)/(1 - x)^7.
 * https://oeis.org/A160829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/36)*(36 + 174*n + 391*n^2 + 513*n^3 + 442*n^4 +
 *            213*n^5 + 67*n^6)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 35, u64 n <= 1465,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit intermediates to avoid overflow for 64‑bit seq_t */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;
    unsigned __int128 n5 = n4 * N;
    unsigned __int128 n6 = n5 * N;

    unsigned __int128 sum = (unsigned __int128)36
        + (unsigned __int128)174 * N
        + (unsigned __int128)391 * n2
        + (unsigned __int128)513 * n3
        + (unsigned __int128)442 * n4
        + (unsigned __int128)213 * n5
        + (unsigned __int128)67  * n6;

    unsigned __int128 result = sum / 36;
    return (seq_t)result;
}
