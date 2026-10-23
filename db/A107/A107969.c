/*
 * A107969  a(n) = (n+1)(n+2)^2*(n+3)(2n+3)(5n^2 + 19n + 20)/720.
 * https://oeis.org/A107969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)(n+2)^2*(n+3)(2n+3)(5n^2 + 19n + 20)/720
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 N = n;
    unsigned __int128 term1 = N + 1u;
    unsigned __int128 term2 = N + 2u;
    unsigned __int128 term3 = N + 3u;
    unsigned __int128 term4 = 2u * N + 3u;
    unsigned __int128 term5 = 5u * N * N + 19u * N + 20u;

    unsigned __int128 product = term1 * term2 * term2 * term3 * term4 * term5;
    product /= (unsigned __int128)720u;

    return (seq_t)product;
}
